#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <pthread.h>

#define SERVER_IS_RUNNING 1
#define LAST_MESSAGE 1729
#define MAX_TO_EXECUTE 8

key_t key;
int msqid;


void MsqidCheck (int result)
{
    if (result < 0)
    {
        printf("Can\'t get msqid\n");
        exit(-1);
    }
}
void KeyCheck (key_t key)
{
    if(key < 0)
    {
        printf("Can\'t generate key\n");
        exit(-1);
    }
}
void MsgRcvCheck (int result)
{
    if(result < 0)
    {
        printf("Can\'t receive message from queue\n");
        exit(-1);
    }
}
void MsgSndCheck( int result)
{
    if(result < 0)
    {
        printf("Can'\t send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
    }
}
void ThreadCreateCheck (int result)
{
    if(result != 0)
    {
        printf ("Error on thread create, return value = %d\n", result);
        exit(-1);
    }
}

void* Process (void* dummy)
{
    struct MyMsgBuf
    {
        long mtype;
        struct Info
        {
            int a;
            int b;
            int mTypeToSend;
            int product;
        } info;
    } msgbuf;


    while(SERVER_IS_RUNNING)
    {
        MsgRcvCheck(msgrcv(msqid, (struct mymsgbuf *) &msgbuf, sizeof(msgbuf.info), 1, 0));
        msgbuf.info.product = msgbuf.info.a * msgbuf.info.b;
        msgbuf.mtype = msgbuf.info.mTypeToSend;
        MsgSndCheck(msgsnd(msqid, &msgbuf, sizeof(msgbuf.info), msgbuf.mtype));
    }
}

int main()
{
    char pathname[] = "/home/ivan/Programming/KeyFile";
    pthread_t* thid = (pthread_t*) calloc (MAX_TO_EXECUTE, sizeof(pthread_t));
    KeyCheck(key = ftok(pathname, 0));
    MsqidCheck(msqid = msgget(key, 0666 | IPC_CREAT));
    printf("msqid = %d, key = %d\n", msqid, key);
    for (int i = 0; i < MAX_TO_EXECUTE; i++) ThreadCreateCheck(pthread_create( &(thid[i]), (pthread_attr_t *)NULL, Process, NULL));
    for (int i = 0; i < MAX_TO_EXECUTE; i++) pthread_join(thid[i], (void **)NULL);

    return 0;
}
