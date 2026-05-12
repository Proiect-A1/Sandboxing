#include <stdio.h>
#include <dirent.h>

int main() {
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            fprintf(stderr, "%s\n", dir->d_name);
        }
        closedir(d);
    }
    return 0;
}