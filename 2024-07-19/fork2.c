#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
int main()
{
 if (fork() == 0)
 {
  printf("Hello from Child \n");
 }
 else if (fork() > 0)
 {
  printf("Hello from Parent \n");
 } 
 else
 {
  printf("Unable to fork \n ");
 }
 return 0;
}
 
