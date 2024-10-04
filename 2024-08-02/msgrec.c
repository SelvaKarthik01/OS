#include<stdio.h>
#include<sys/ipc.h>
struct queue
{
 long type;
 char text[100];
}buf;
long int x;
int i = 1;
int main()
{
 int msgqid;
 msgqid = msgget(42L,0666|IPC_CREAT);
 printf("%d",msgqid);
 while(i<=5)
 {
  printf("Enter the Type : ");
  scanf("%ld",&buf.type);
  printf("%ld",buf.type);
  msgrcv(msgqid,&buf,sizeof(buf.text),buf.type,0);
  printf("Message associated with type is %s",buf.text);
  i++;
 }
 return 0;
}

 
