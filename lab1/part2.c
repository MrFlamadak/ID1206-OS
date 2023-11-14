#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#define MSGSIZE 8192

int main() {
    struct mq_attr attr;
    mqd_t mqd;
    const char *mq_name = "/mqd";
    //scratch

    pid_t pid = fork();
    switch (pid)
    {
    case -1:
        printf("There has been an error in process creation.");
        break;
    case 0://child Producer
        attr.mq_maxmsg = 10;
        attr.mq_msgsize = MSGSIZE;
        mqd = mq_open(mq_name, O_WRONLY | O_CREAT, 0666, 0);
        //file reading
        FILE *file = fopen("sometext.txt", "r");
        char sendBuffer[MSGSIZE];
        fread(sendBuffer, 1, sizeof(sendBuffer), file);
        mq_send(mqd, sendBuffer, MSGSIZE, 0);

        mq_close(mqd);
        break;

    default://parent Consumer
        wait(NULL);
        mqd = mq_open(mq_name, O_RDONLY);
        char ReceiveBuffer[MSGSIZE];
        
        mq_receive(mqd, ReceiveBuffer, MSGSIZE, 0);

        printf("Message from queue:\n%s", ReceiveBuffer);

        mq_close(mqd);
        mq_unlink(mq_name);
        exit(0);        
        break;
    }
    
}