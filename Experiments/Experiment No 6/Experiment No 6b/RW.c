#include<stdio.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<string.h>
#include<unistd.h>
#define SHMSZ 1024
int main()
{
key_t key1 , key2;
int shmid;
int sec_id;
char *shm , *k , *s;//Data
char *ssm, *j , *t;
key2 = 3400; //Pattern
key1 = 3415;//Data
if((shmid = shmget(key1, SHMSZ, IPC_CREAT | 0666))<0)
{
perror("shmget");
exit(1);
}

if((shm = shmat(shmid,NULL,0)) == (char *)-1)
{
perror("shmat");
exit(1);
}
//***************************************************************
if((sec_id = shmget(key2, SHMSZ, IPC_CREAT | 0666))<0)
{
perror("shmget");
exit(1);
}
if((ssm = shmat(sec_id,NULL,0)) == (char *)-1)
{
perror("shmat");
exit(1);
}
t = ssm;
char pattern[10];
printf("\nEnter the pattern: ");
fgets(pattern,10, stdin); //Will accept till ENTER key is hit...
int len = sizeof(pattern);
for(int i = 0; i < len; i++)
{
*t = pattern[i];
*t++;
}
for (k = ssm; *k != '\0'; k++)
printf("%c",*k);
putchar('\n');

//Reader Priority
int choice;
printf("1. READER PRIORITY \n 2. WRITER PRIORITY\n");
printf("\nEnter the priority (1/2): ");
scanf("%d",&choice);
if(choice == 1)
{
char first = *ssm;
printf("\nThe first character is: %c",first);
if(first == 'R')
{
int rcount = 0;
int wcount = 0;
for (k = ssm; *k != '\0'; k++)
{
if(*k == 'R')
rcount = rcount + 1;
else if(*k == 'W')
wcount = wcount + 1;

}
printf("\nThe no. of readers is: %d",rcount);
printf("\nThe no. of writer is: %d\n",wcount);
for(int i = 0; i < rcount; i++)
{
system("gnome-terminal -- ./r.out");
sleep(2);
}

for(int i = 0; i < wcount; i++)
{
system("gnome-terminal -- ./w.out");
sleep(10);
}
}
else
{
char* h;
h = ssm;
do
{
system("gnome-terminal -- ./w.out");
sleep(10);
h++;
}while(*h != 'R');
int rcount = 0;
int wcount = 0;
for (k = h; *k != '\0'; k++)
{
if(*k == 'R')
rcount = rcount + 1;
else if(*k == 'W')
wcount = wcount + 1;

}
printf("\nThe no. of readers is: %d",rcount);
printf("\nThe no. of writer is: %d\n",wcount);
for(int i = 0; i < rcount; i++)
{
system("gnome-terminal -- ./r.out");

sleep(2);
}
for(int i = 0; i < wcount; i++)
{
system("gnome-terminal -- ./w.out");
sleep(10);
}
}
}
else if(choice == 2)
{
char first = *ssm;
printf("\nThe first character is: %c",first);
if(first == 'W')
{
int rcount = 0;
int wcount = 0;
for (k = ssm; *k != '\0'; k++)
{
if(*k == 'R')
rcount = rcount + 1;
else if(*k == 'W')
wcount = wcount + 1;

}
printf("\nThe no. of readers is: %d",rcount);
printf("\nThe no. of writer is: %d\n",wcount);
for(int i = 0; i < wcount; i++)
{
system("gnome-terminal -- ./w.out");
sleep(10);

}
for(int i = 0; i < rcount; i++)
{
system("gnome-terminal -- ./r.out");
sleep(2);
}
}
else
{
char* h;
h = ssm;
do
{
system("gnome-terminal -- ./r.out");
sleep(2);
h++;
}while(*h != 'W');
int rcount = 0;
int wcount = 0;
for (k = h; *k != '\0'; k++)
{
if(*k == 'R')
rcount = rcount + 1;
else if(*k == 'W')
wcount = wcount + 1;

}
printf("\nThe no. of readers is: %d",rcount);
printf("\nThe no. of writer is: %d\n",wcount);
sleep(3);

for(int i = 0; i < wcount; i++)
{
system("gnome-terminal -- ./w.out");
sleep(10);
}
for(int i = 0; i < rcount; i++)
{
system("gnome-terminal -- ./r.out");
sleep(2);
}
}
}
return 0;
}




