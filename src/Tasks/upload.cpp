#include <Tasks/upload.h>

upload_task::upload_task(std::string url, std::string problem_id, int rev_id)
{
    this->url = url;
    this->problem_id = problem_id;
    this->rev_id = rev_id;
}

bool upload_task::check_permissions(int user_id) { return true; }

int upload_task::callback_http(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
{
    upload_data *data = (struct upload_data *)lws_get_opaque_user_data(wsi);

    switch (reason) {
        case LWS_CALLBACK_CLIENT_APPEND_HANDSHAKE_HEADER: {
            unsigned char **p = (unsigned char **)in;
            unsigned char *end = (*p) + len;

            if (lws_add_http_header_by_token(wsi, WSI_TOKEN_HTTP_CONTENT_TYPE,
                                             (unsigned char *)"application/zip", 15, p, end)) {
                return -1;
            }

            char clen[32];
            int n = snprintf(clen, sizeof(clen), "%lld", (long long)data->file_size);
            if (lws_add_http_header_by_token(wsi, WSI_TOKEN_HTTP_CONTENT_LENGTH,
                                             (unsigned char *)clen, n, p, end)) {
                return -1;
            }

            if (lws_add_http_header_by_token(wsi, WSI_TOKEN_CONNECTION,
                                             (unsigned char *)"close", 5, p, end)) {
                return -1;
            }

            lws_client_http_body_pending(wsi, 1);
            lws_callback_on_writable(wsi);
            break;
        }

        case LWS_CALLBACK_CLIENT_HTTP_WRITEABLE: {
            if (!data || data->fd < 0) return -1;

            unsigned char buf[LWS_PRE + 4096];
            ssize_t n = read(data->fd, buf + LWS_PRE, 4096);

            if (n < 0) {
                fprintf(stderr, ">>> Error reading file\n");
                return -1;
            }

            if (n == 0) {
                lws_client_http_body_pending(wsi, 0);
                return 0;
            }

            enum lws_write_protocol wp = LWS_WRITE_HTTP;
            if (n < 4096) {
                off_t cur = lseek(data->fd, 0, SEEK_CUR);
                if (cur == data->file_size) {
                    wp = LWS_WRITE_HTTP_FINAL;
                    lws_client_http_body_pending(wsi, 0);
                }
            }

            if (lws_write(wsi, buf + LWS_PRE, n, wp) != n) {
                fprintf(stderr, ">>> lws_write failed\n");
                return -1;
            }

            if (wp != LWS_WRITE_HTTP_FINAL) {
                lws_callback_on_writable(wsi);
            }

            printf(">>> Sent chunk of %zd bytes...\n", n);
            break;
        }

        case LWS_CALLBACK_ESTABLISHED_CLIENT_HTTP: {
            int status = lws_http_client_http_response(wsi);
            printf(">>> HTTP Status: %d\n", status);
            if (status < 200 || status >= 300) {
                fprintf(stderr, ">>> Upload rejected by server. %d\n", status);
                return -1;
            }
            break;
        }

        case LWS_CALLBACK_RECEIVE_CLIENT_HTTP: {
            char buffer[8192 + LWS_PRE];
            char *px = buffer + LWS_PRE;
            int lenx = sizeof(buffer) - LWS_PRE;
            if (lws_http_client_read(wsi, &px, &lenx) < 0) return -1;
            return 0;
        }

        case LWS_CALLBACK_RECEIVE_CLIENT_HTTP_READ:
            return 0;

        case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
            fprintf(stderr, ">>> CLIENT_CONNECTION_ERROR\n");
            data->interrupted = 1;
            break;

        case LWS_CALLBACK_COMPLETED_CLIENT_HTTP:
        case LWS_CALLBACK_CLOSED_CLIENT_HTTP:
        case LWS_CALLBACK_CLOSED:
        case LWS_CALLBACK_WSI_DESTROY:
            printf(">>> Connection closed. Event loop terminating.\n");
            data->interrupted = 1;
            break;

        default:
            break;
    }
    return lws_callback_http_dummy(wsi, reason, user, in, len);
}

bool upload_task::prepare()
{
    std::string pdf = architecture_utilities::get_problem_data_folder(problem_id , rev_id);
    std::string pza = architecture_utilities::get_problem_zip(problem_id , rev_id);

    if(!general_utilities::is_file(pza))
    {
        LOG_ERROR("Trying to upload file: not existent archive");
        return false;
    }

    if(!general_utilities::is_folder(pdf))
    {
        LOG_ERROR("Trying to upload file: not existent problem folder");
        return false;
    }

    int status_code = system((std::string("zip -r ") + pza + " " + pdf).c_str());

    if(status_code != 0)
    {
        LOG_ERROR("Zip failed");
        return false;
    }

    return true;
}

result_enum upload_task::execute(pthread_t thread_id, int user_id)
{
    if(prepare() == 0)
    {
        LOG_ERROR("Preparing failed");
        return result_enum::FAIL;
    }

    LOG_DEBUG("Started uploading");
    struct lws_context_creation_info info;
    struct lws_client_connect_info i;
    struct lws_context *context;
    bool interrupted = false;

    lws_set_log_level(LLL_ERR | LLL_WARN | LLL_NOTICE | LLL_USER, NULL);

    char zip_path[PATH_MAX];
    sprintf(zip_path, "%s/tmp/%s.%d", getenv("SANDBOX_PATH"), problem_id.c_str(), rev_id);

    int fd = open(zip_path, O_RDONLY | O_BINARY);
    if (fd < 0) {
        perror("Failed to open zip file");
        return result_enum::FAIL;
    }

    off_t file_size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    upload_data client_data = { fd, file_size, interrupted };

    memset(&info, 0, sizeof info);
    info.options = LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;
    info.port = CONTEXT_PORT_NO_LISTEN;
    info.protocols = protocols;

    context = lws_create_context(&info);
    if (!context) {
        LOG_ERROR("Failed to create lws context\n");
        close(fd);
        return result_enum::FAIL;
    }

    const char *protocol, *address, *path;
    int port;

    std::cerr << "beforeee " << url << std::endl;
    
    std::string url_cp = url;

    if (lws_parse_uri((char *)url_cp.c_str(), &protocol, &address, &port, &path)) {
        LOG_ERROR("Failed to parse URL\n");
        close(fd);
        lws_context_destroy(context);
        return result_enum::FAIL;
    }

    char full_path[4096];
    snprintf(full_path, sizeof(full_path), "/%s", path);

    std::cerr << "Upload" << std::endl;
    std::cerr << full_path << std::endl << address << std::endl << url << std::endl;

    memset(&i, 0, sizeof i);
    i.context = context;
    i.address = address;
    i.port = 443;
    i.path = full_path;
    i.host = address;
    i.origin = address;
    i.method = "POST";
    i.alpn = "http/1.1";
    i.protocol = "http";
    i.opaque_user_data = &client_data;
    i.ssl_connection = LCCSCF_USE_SSL | LCCSCF_ALLOW_SELFSIGNED | LCCSCF_SKIP_SERVER_CERT_HOSTNAME_CHECK | LCCSCF_ALLOW_EXPIRED;

    if (!lws_client_connect_via_info(&i)) {
        LOG_ERROR("Failed to initiate connection\n");
        lws_context_destroy(context);
        close(fd);
        return result_enum::FAIL;
    }

    while (!client_data.interrupted) {
        lws_service(context, 1000);
    }

    close(fd);
    lws_context_destroy(context);

    LOG_DEBUG("Uploaded successfully");
    return result_enum::OK;
}