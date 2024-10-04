
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#define MAX 10
// structure for message queue
struct mesg_buffer {
long mesg_type;
char mesg_text[100];
} message;
int main()

{
key_t key;
int msgid,type;
char choice[10];
key = ftok("progfile", 65);
msgid = msgget(key, 0666 | IPC_CREAT);
printf("Read Data : ");
do{
printf("Enter the type of the message: ");
scanf("%d",&type);
msgrcv(msgid, &message, sizeof(message),type, 0);
printf("Message is : %s \n", message.mesg_text);
printf("Do you want to continue: ");
scanf("%s",choice);
}while(strcmp(choice,"no")!=0);
msgctl(msgid, IPC_RMID, NULL);
return 0;
}
