#include <stdio.h>
#include <stdlib.h>

int main() {
    int x = 0;
    if (scanf("%d", &x) == 1) {
        size_t size = (size_t)x * 10 * 1024 * 1024;
        if (size < 100) size = 100;
        char *v = (char *)malloc(size);
        if (v) {
            for (size_t i = 0; i < size; i += 100) {
                v[i] = (char)(i % 256);
            }
            printf("%d\n", v[size / 2]);
            free(v);
        }
    }
    return 0;
}