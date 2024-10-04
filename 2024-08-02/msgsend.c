// Program fro Sendind and Receiving MessagesQueues Between Processes

#include<stdio.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<string.h>
struct queue
{
 long type;
 char text[200];
}buf;
int main()
{
 int msgqid;
 msgqid = msgget(42L,0666|IPC_CREAT);
 printf("Message qid : %d",msgqid);
 printf("Enter the Message");
 scanf("%s",buf.text);
 do
 {
  printf("Enter the Message Type : ");
  scanf("%ld",&buftype);
  int len = strlen(buf.text);
  msgsend(msgqid,&buf,len+1,0);
  printf("Enter the Message");
  scanf("%s",buf.text);
 }while(strcmp(buf.text,"end")!= 0);
 msgctl(msgqid,IPC_RMID,NULL);
 return 0;
}
