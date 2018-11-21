#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define LAST_MESSAGE 1729

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
void SendToServer (int a, int b)
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

    msgbuf.mtype = 1;
    msgbuf.info.a = a;
    msgbuf.info.b = b;
    msgbuf.info.mTypeToSend = getpid();
    MsgSndCheck(msgsnd(msqid, &msgbuf, sizeof(msgbuf.info), msgbuf.mtype));
}
void RecieveFromServer()
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
    msgbuf.mtype = getpid();
    MsgRcvCheck(msgrcv(msqid, (struct mymsgbuf *) &msgbuf, sizeof(msgbuf.info), msgbuf.mtype, 0));
    printf("The answer is %d\n", msgbuf.info.product);
    //msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
}
int main()
{
    int a, b;
    char pathname[] = "/home/ivan/Programming/KeyFile";
    KeyCheck(key = ftok(pathname, 0));
    MsqidCheck(msqid = msgget(key, 0666 | IPC_CREAT));
    printf("msqid = %d, key = %d\n", msqid, key);
    printf("Enter two numbers to product:\n");
    scanf("%d %d", &a, &b);
    SendToServer(a, b);
    RecieveFromServer();
}
