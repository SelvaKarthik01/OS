//Pipe 2-way Communication 
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
int main()
{
 int p1[2];
 int p2[2];
 if(pipe(p1) == -1)
 {
  return -1;
 }
 if(pipe(p2) == -1)
 {
  return -1;
 }
 int pid = fork();
 if(pid == -1)
 {
  return 2;
 }
 if (pid == 0)
 {
  close(p1[0]);
  close(p2[1]);
  int x;
  if(read(p2[0],&x,sizeof(x)) == -1)
  {
   return 3;
  }
  printf("received %d\n",x);
  x *= 4;
  if(write(p1[1],&x,sizeof(x))==-1)
  {
   return 4;
  }
  printf("Wrote %d\n",x);
  close(p1[1]);
  close(p2[0]);
  srand(time(NULL));
  int y = rand() % 10;
  if(write(p2[1],&y,sizeof(y)==-1))
  {
   return 5;
  }
  if(read(p1[0],&y,sizeof(y))==-1)
  {
   return 6;
  }
  printf("Result is %d\n",y);
  close(p1[0]);
  close(p2[1]);
  wait(NULL);
 }
 return 0;
}
