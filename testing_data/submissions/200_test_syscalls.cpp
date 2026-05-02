#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>

void test_create_delete_file(const char* file_path) {
    fprintf(stderr, "creez fisier %s... ", file_path);
    int fd = open(file_path, O_CREAT | O_TRUNC, 0600);

    if (fd == -1) {
        perror("fail");
    } else {
        fprintf(stderr, "ok\n");

        fprintf(stderr, "inchid fisier %s... ", file_path);
        int ret = close(fd);
        
        if (ret == -1) {
            perror("fail");
        } else {
            fprintf(stderr, "ok\n");

            fprintf(stderr, "sterg fisier %s... ", file_path);
            ret = unlink(file_path);

            if (ret == -1) {
                perror("fail");
            } else {
                fprintf(stderr, "ok\n");
            }
        }
    }
}

void test_nonexistent_syscall(long number) {
    fprintf(stderr, "apelez syscall %ld... ", number);
    int ret = syscall(number);

    if (ret == -1) {
        perror("fail");
    } else {
        fprintf(stderr, "ok\n");
    }
}

void test_forks() {
    fprintf(stderr, "forking...\n");
    pid_t pid = fork();

    if (pid == -1) {
        perror("forking failed");
    } else if (pid == 0) {
        fprintf(stderr, "forking worked. hello from copil\n");
        exit(0);
    } else {
        fprintf(stderr, "forking worked. hello from parinte\n");
        wait(NULL);
    }
}

// daca apelezi asta e bye bye pcu tau
void test_fork_bomb() {
    fprintf(stderr, "boom forking...\n");
    pid_t pid;

    while (1) {
        pid = fork();
        if (pid == -1) {
            perror("boom forking failed");
            return;
        } else {
            fprintf(stderr, "%d says: you're cooked\n", getpid());
        }
    }
}

void test_mkdir(const char* dir_path) {
    fprintf(stderr, "creez folder %s... ", dir_path);
    int fd = mkdir(dir_path, 0600);

    if (fd == -1) {
        perror("fail");
    } else {
        fprintf(stderr, "ok\n");
    }
}

void test_socket(const char *url) {
    const char *host = url;
    const char *port = "80";

    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(host, port, &hints, &res) != 0) {
        perror("getaddrinfo");
        return;
    }

    int sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock < 0) {
        perror("socket");
        return;
    }

    if (connect(sock, res->ai_addr, res->ai_addrlen) < 0) {
        perror("connect");
        return;
    }

    // HTTP request (simple)
    const char *msg =
        "GET / HTTP/1.1\r\n"
        "Host: ezluci.com\r\n"
        "Connection: close\r\n"
        "\r\n";

    send(sock, msg, strlen(msg), 0);

    // read response
    char buffer[4096];
    int n;

    while ((n = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[n] = '\0';
        fprintf(stderr, "%s", buffer);
    }

    if (n < 0) {
        perror("recv");
    }

    close(sock);
    freeaddrinfo(res);
}

void test_exec() {
    fprintf(stderr, "exec bash...");
    char* args[] = {"/bin/bash", "--version", NULL};
    
    dup2(2, 1); // stdout devine stderr
    execv("/bin/bash", args);

    perror("exec fail");
}

int main() {
    int x;
    if (scanf("%d", &x) == 1 && x != 0) {
        exit(0); // vreau un singur test, nu 10
    }

    test_create_delete_file("/my_file");
    test_create_delete_file("./my_file");

    test_nonexistent_syscall(999);

    test_forks();
    // test_fork_bomb();

    test_mkdir("/my_folder");
    test_mkdir("./my_folder");

    test_socket("ezluci.com");

    test_exec();
}