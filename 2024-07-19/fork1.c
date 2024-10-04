#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
int main()
{
 fork();
 fork();
 fork();
 printf("Hello \n PID = %d \n",getpid());
 return 0;
}
