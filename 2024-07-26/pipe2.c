#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
int main()
{
int p[2];
pid_t pid;
char inbuf[10],outbuf[10];
if(pipe(p)==-1)
{
printf("pipe failed\n");
return 1;
}
else
printf(" pipe created\n");
if((pid=fork()))
{
printf("In parent process\n");
printf("type the data to be sent to child:");
scanf("%s",outbuf);
write (p[1],outbuf,10);
sleep(10);
printf("after sleep in parent process\n");
}
else
{

printf("In child process\n");               
read(p[0],inbuf,10);
printf("the data received by the child is %s\n",inbuf);
sleep(10);
printf("After sleep in child\n");
}
return 0;
}

