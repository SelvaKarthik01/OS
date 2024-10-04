#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
int main()
{
 pid_t p;
 p = fork();
 if(p == 0)
 {
  sleep(5);
  printf("Im child having PID %d\n",getpid());
  printf("My Parent PID is %d\n",getppid());
 }
 else
 {
  printf("I am Parent having PID %d\n",getpid());
  printf("My Child's PID is %d \n",p);
 }
 return 0;
}
