#ifndef UPLOAD_TASK_H
#define UPLOAD_TASK_H

#include <libwebsockets.h>
#include <string>
#include <Enums/result_enum.h>
#include <Tasks/runner_factories.hpp>
#include <Tasks/task.h>
#include <Utilities/architecture_utilities.h>
#ifndef O_BINARY
#define O_BINARY 0
#endif

class upload_task : public task
{
    private:
        std::string url;
        std::string problem_id;
        int rev_id;

        const lws_protocols protocols[2] = { { "http", callback_http, 0, 65536, 0, NULL, 0 }, { NULL, NULL, 0, 0, 0, NULL, 0 }};
        static int callback_http(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len);
        bool check_permissions(int user_id) override;
        bool prepare();
    public: 
        result_enum execute(pthread_t thread_id, int user_id) override;
        upload_task(std::string url , std::string problem_id , int rev_id);
};

struct upload_data 
{
    int fd;
    off_t file_size;
    bool &interrupted;
};
#endif // UPLOAD_H