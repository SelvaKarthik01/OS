//Writer with Priority
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include <sys/shm.h>
#include<stdbool.h>
#define SHMSZ 1024
#define MAX_LIMIT 1024
int main()
{
int shmid;

key_t key;
key = 3415;
char *shm , *s , *k;
if ((shmid = shmget(key, SHMSZ, 0666))<0)
{
perror("shmget");
exit(1);
}
if ((shm = shmat(shmid, NULL, 0)) == (char *) -1)
{
perror("shmat");
exit(1);
}
s = shm;
printf("\n%d is the only Write process that is accessing the Shared Memory...",getpid());
bool temp = true;
while(true)
{

if(*s != '\0')
++s;
else if(*s == '\0')
break;

}
char x = *s;

printf("\nThe Data here should be null: %c",x);
char str[MAX_LIMIT];
int choice;
do
{
printf("\nEnter the String to be entered into the Shared Memory: ");
fgets(str, MAX_LIMIT, stdin); //Will accept till ENTER key is hit...
printf("\nAre you sure the string you want to enter is: %s ",str);
printf("\nIf YES -> 1 | NO -> 0: ");
scanf("%d",&choice);
}while(choice != 1);
printf("The String that will be entered into the SHM is: %s\n", str);
for(int i = 0 ; i < sizeof(str) ; i++)
{
*s++ = str[i]; //After the last mem. location, s will point to NULL...
}
sleep(5);
printf("\nThe Write process for %d Process is complete!!!\n",getpid());
for (k = s; *k != '\0'; k++)
{
printf("%c",*k);
}
putchar('\n');
return 0;
}

