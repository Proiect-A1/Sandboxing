#include <Tasks/download.h>
#include <Tasks/preparator.h>
#include <Singletoni/task_queue.h>

download_task::download_task(std::string url , std::string problem_id , int rev_id)
{
    this -> url = url;
    this -> problem_id = problem_id;
    this -> rev_id = rev_id;
}

bool download_task::check_permissions(int user_id){return true;}

int download_task::callback_http(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
{
    download_data *data = (struct download_data *)lws_get_opaque_user_data(wsi);

    switch (reason) {
        case LWS_CALLBACK_CLIENT_APPEND_HANDSHAKE_HEADER: {
            unsigned char **p = (unsigned char **)in;
            unsigned char *end = (*p) + len;
            if (lws_add_http_header_by_token(wsi, WSI_TOKEN_CONNECTION,
                                             (unsigned char *)"close", 5, p, end)) {
                return -1;
            }
            break;
        }

        case LWS_CALLBACK_ESTABLISHED_CLIENT_HTTP: {
            int status = lws_http_client_http_response(wsi);
            printf(">>> HTTP Status: %d\n", status);
            if (status != 200) {
                fprintf(stderr, ">>> Download rejected by server.%d\n" , status);
                return -1; 
            }
            break;
        }

        case LWS_CALLBACK_RECEIVE_CLIENT_HTTP: {
            char buffer[8192 + LWS_PRE];
            char *px = buffer + LWS_PRE;
            int lenx = sizeof(buffer) - LWS_PRE;

            if (lws_http_client_read(wsi, &px, &lenx) < 0) {
                return -1;
            }
            return 0; 
        }

        case LWS_CALLBACK_RECEIVE_CLIENT_HTTP_READ:
            if (data && data->fd >= 0) {
                ssize_t bytes_written = write(data->fd, in, len);
                if (bytes_written < 0) {
                    fprintf(stderr, ">>> Error writing to fd\n");
                    return -1; 
                }
                printf(">>> Downloaded chunk of %zu bytes...\n", len); 
            }
            return 0; 
            
        case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
            fprintf(stderr, ">>> CLIENT_CONNECTION_ERROR\n");
            data -> interrupted = 1;
            break;

        case LWS_CALLBACK_COMPLETED_CLIENT_HTTP:
        case LWS_CALLBACK_CLOSED_CLIENT_HTTP:
        case LWS_CALLBACK_CLOSED:
        case LWS_CALLBACK_WSI_DESTROY:
            printf(">>> Connection closed. Event loop terminating.\n");
            data -> interrupted = 1;
            break;
            
        default:
            break;
    }
    return lws_callback_http_dummy(wsi, reason, user, in, len);
}

result_enum download_task::execute(pthread_t thread_id , int user_id)
{
    LOG_DEBUG("Started downloading");
    struct lws_context_creation_info info;
    struct lws_client_connect_info i;
    struct lws_context *context;
    bool interrupted;

    lws_set_log_level(LLL_ERR | LLL_WARN | LLL_NOTICE | LLL_USER, NULL);

    char zip_path[PATH_MAX];
    sprintf(zip_path , "%s/tmp/%s.%d" , getenv("SANDBOX_PATH") , problem_id.c_str() , rev_id);    

    download_data client_data = {open(zip_path , O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, 0600) , interrupted};

    if (client_data.fd < 0) {
        perror("Failed to open file");
        return result_enum::FAIL;
    }

    memset(&info, 0, sizeof info);
    info.options = LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;
    info.port = CONTEXT_PORT_NO_LISTEN;
    info.protocols = protocols;

    context = lws_create_context(&info);
    if (!context) {
        LOG_ERROR("Failed to create lws context\n");
        close(client_data.fd);
        return result_enum::FAIL;
    }
    
    const char *protocol, *address, *path;
    int port;

    std::string url_cp = url;

    if (lws_parse_uri((char *) url_cp.c_str(), &protocol, &address, &port, &path)) {
        LOG_ERROR("Failed to parse URL\n");
        return result_enum::FAIL;
    }

    char full_path[4096];
    snprintf(full_path, sizeof(full_path), "/%s", path);

    std::cerr << full_path << std::endl << address << std::endl;

    memset(&i, 0, sizeof i);
    i.context = context;
    i.address = address;
    i.port = port;
    i.path = full_path;
    i.host = address;
    i.origin = address;
    i.method = "GET";
    i.alpn = "http/1.1";
    i.protocol = "http";
    i.opaque_user_data = &client_data;
    i.ssl_connection = LCCSCF_USE_SSL | LCCSCF_ALLOW_SELFSIGNED | LCCSCF_SKIP_SERVER_CERT_HOSTNAME_CHECK | LCCSCF_ALLOW_EXPIRED;
    i.port = 443;
    
    if (!lws_client_connect_via_info(&i)) {
        LOG_ERROR("Failed to initiate connection\n");
        lws_context_destroy(context);
        close(client_data.fd);
        return result_enum::FAIL;
    }

    while (!interrupted) {
        lws_service(context, 1000);
    }

    close(client_data.fd);
    lws_context_destroy(context);
    
    preparator *prep = new preparator(problem_id , rev_id);
    prep -> priority = 1000000;
    task_queue::get_instance().push(prep);

    LOG_DEBUG("Downloaded successfully");
    return result_enum::OK;
}