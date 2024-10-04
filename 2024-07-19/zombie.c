#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
int main()
{
 pid_t child_pid = fork();
 child_pid = fork();
 if(child_pid > 0)
 {
  printf("Child Process in Deep Sleep for 60 seconds !");
  sleep(60);
 }
 else
 {
  exit(0);
 }
 return 0;
}
