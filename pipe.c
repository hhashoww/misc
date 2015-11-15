#include <stdio.h>
#include <unistd.h>

int main() {

    int pipefd[2];
    char buf[1024];

    pipe(pipefd);

    switch(fork()) {
        case -1:
            perror("fork error\n");
            return -1;
        case 0:
            printf("child close read\n");
            close(pipefd[0]);
            write(pipefd[1], "YOYOYO", sizeof("YOYOYO"));
            break;
        default:
            printf("parent close write\n");
            close(pipefd[1]);
            read(pipefd[0], buf, 1024);
            printf("str = %s\n", buf);
    }
    return 0;
}
