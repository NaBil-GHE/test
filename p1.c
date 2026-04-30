#include <stdio.h>
#include <unistd.h>

int main() {
    if (fork() && fork()) {
        printf("X\n");
    }
    return 0;
}