// Pipe using one-way Communication 
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
int main()
{
 int p[10];
 char input[20],output[20];
 pipe(p);
 int pid = fork();
 if (pid > 0)
 {
  printf("In Parent Process Communication ;)\n");
  printf("Enter the Data to be sent to the Child through parents : \n");
  scanf("%s",output);
  write(p[1],output,sizeof(output));
  sleep(60);
 }
 else if (pid < 0)
 {
  printf("Unable to Create a Child Process : ( \n");
 }  
 else
 {
  sleep(6);
  printf("In Child \n");
  read(p[0],input,10);
  printf("Data received by the Child is : %s \n",input);\
 }
 return 0;
}
