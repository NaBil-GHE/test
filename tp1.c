#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>




int main(){
    int pid;
    int fd[2],fd2[2];
    if(pipe(fd) == -1) {
        perror("pipe error");
        exit(1);
    }

    if(pipe(fd2) == -1) {
        perror("pipe error");
        exit(1);
    }

    pid = fork();
    if(pid == 0) {
        close(fd[1]);
        close(fd2[0]);
        char m[100];
        read(fd[0], m, sizeof(m));
        printf("message recu(fils): %s \n", m);
        printf("message a envoyer(fils): Hi \n");
        write(fd2[1], "Hi", sizeof("Hi"));
        close(fd[0]);
        close(fd2[1]);

    } else {
        close(fd[0]);
        printf("message a envoyer(pere): hello \n");
        write(fd[1], "hello", sizeof("hello"));

     
        wait(NULL);

   char m2[100];
        read(fd2[0], m2, sizeof(m2));
        printf("message recu(pere): %s \n", m2);
        close(fd2[0]);



        close(fd[1]);

       
    }



    return 0;



}