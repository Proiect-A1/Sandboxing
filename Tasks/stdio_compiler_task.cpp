#include <stdio_compiler_task.h>

#include <chrono>
#include <csignal>
#include <cstdlib>
#include <filesystem>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

namespace
{
    bool copy_file(const std::string &from, const std::string &to, mode_t mode)
    {
        std::error_code ec;
        std::filesystem::copy_file(
            from,
            to,
            std::filesystem::copy_options::overwrite_existing,
            ec);
        if (ec)
        {
            return false;
        }

        if (chmod(to.c_str(), mode) != 0)
        {
            return false;
        }

        return true;
    }
}

bool stdio_compiler_task::check_permissions()
{
    if (compile_command.empty() || source_file_name.empty() || output_file_name.empty())
    {
        return false;
    }
    if (submission_id <= 0)
    {
        return false;
    }
    if (time_limit <= 0 || exec_size_limit <= 0)
    {
        return false;
    }
    return true;
}

result_enum stdio_compiler_task::execute(int thread_id, int user_id)
{
    (void)thread_id;

    if (!check_permissions())
    {
        return FAIL;
    }
    if (user_id <= 0)
    {
        return FAIL;
    }

    const char *sandbox_path = getenv("SANDBOX_PATH");
    if (sandbox_path == nullptr || sandbox_path[0] == '\0')
    {
        return FAIL;
    }

    const std::string run_username = "amarat" + std::to_string(user_id);
    const std::string run_dir = std::string(sandbox_path) + "/runs/" + run_username;
    const std::string submissions_dir = std::string(sandbox_path) + "/submissions/" + std::to_string(submission_id);

    const std::string source_host_path = submissions_dir + "/" + source_file_name;
    const std::string output_host_path = submissions_dir + "/" + output_file_name;
    const std::string source_run_path = run_dir + "/" + source_file_name;
    const std::string output_run_path = run_dir + "/" + output_file_name;

    struct passwd *pw = getpwnam(run_username.c_str());
    if (pw == nullptr)
    {
        return FAIL;
    }

    if (!copy_file(source_host_path, source_run_path, 0644))
    {
        return FAIL;
    }

    unlink(output_run_path.c_str());

    const auto started_at = std::chrono::steady_clock::now();
    pid_t pid = fork();
    if (pid < 0)
    {
        return FAIL;
    }

    if (pid == 0)
    {
        setpgid(0, 0);

        if (chdir(run_dir.c_str()) != 0)
        {
            _exit(127);
        }

        if (chroot(run_dir.c_str()) != 0)
        {
            _exit(127);
        }

        if (chdir("/") != 0)
        {
            _exit(127);
        }

        if (initgroups(run_username.c_str(), pw->pw_gid) != 0)
        {
            _exit(127);
        }
        if (setgid(pw->pw_gid) != 0)
        {
            _exit(127);
        }
        if (setuid(pw->pw_uid) != 0)
        {
            _exit(127);
        }

        int null_fd = open("/dev/null", O_RDONLY);
        if (null_fd >= 0)
        {
            dup2(null_fd, STDIN_FILENO);
            close(null_fd);
        }

        std::string source_in_jail = "/" + source_file_name;
        std::string output_in_jail = "/" + output_file_name;

        char *const argv[] = {
            const_cast<char *>(compile_command.c_str()),
            const_cast<char *>("-std=c++17"),
            const_cast<char *>("-O2"),
            const_cast<char *>("-o"),
            const_cast<char *>(output_in_jail.c_str()),
            const_cast<char *>(source_in_jail.c_str()),
            nullptr};
        execv(compile_command.c_str(), argv);
        _exit(127);
    }

    setpgid(pid, pid);

    int status = 0;
    bool timed_out = false;

    while (true)
    {
        pid_t waited = waitpid(pid, &status, WNOHANG);
        if (waited == pid)
        {
            break;
        }
        if (waited < 0)
        {
            killpg(pid, SIGKILL);
            waitpid(pid, &status, 0);
            return FAIL;
        }

        const auto now = std::chrono::steady_clock::now();
        const auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - started_at).count();
        if (elapsed_ms > (long long)time_limit)
        {
            timed_out = true;
            killpg(pid, SIGKILL);
            waitpid(pid, &status, 0);
            break;
        }

        usleep(1000);
    }

    if (timed_out)
    {
        return TLE;
    }

    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
    {
        return CPE;
    }

    struct stat st;
    if (stat(output_run_path.c_str(), &st) != 0)
    {
        return CPE;
    }
    if ((long)st.st_size > exec_size_limit)
    {
        unlink(output_run_path.c_str());
        return CPE;
    }

    if (rename(output_run_path.c_str(), output_host_path.c_str()) != 0)
    {
        if (!copy_file(output_run_path, output_host_path, 0755))
        {
            return FAIL;
        }
        unlink(output_run_path.c_str());
    }

    chmod(output_host_path.c_str(), 0755);
    return OK;
}
