#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>


#define READ_END 0
#define WRITE_END 1

int main(){
    // 1 writes, 0 reads
    int fd[2];
    //pipe created
    pipe(fd);
    

    char *args1[4] = { "/bin/sh", "-c", "ls /", NULL};
    char *args2[4] = { "/bin/sh", "-c", "wc -l", NULL};
    int ret;

    switch(fork()){
        case -1:
            perror("We could not create a new process!");
            exit(1);
        // child
        case 0:
        // ls function 
            close(fd[READ_END]);
            //char *args[2] = {"ls", NULL};
            dup2(fd[WRITE_END], STDOUT_FILENO); //takes input from fd and prints out to standard output
            close(fd[WRITE_END]);
            execv(args1[0], args1); 
        break;
       // parent
       default:
            // to count up the files/directories which are returned from the cild
            wait(NULL);
            close(fd[WRITE_END]);
            dup2(fd[READ_END], STDIN_FILENO);
            close(fd[READ_END]);
            execv(args2[0], args2);
       break;
    }
    return 0;

}