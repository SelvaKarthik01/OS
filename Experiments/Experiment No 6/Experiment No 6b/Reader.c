#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include <sys/shm.h>
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
char flag = *shm;
s = shm;
printf("\nThe data at the first location of the Shared Memory is: %c\n",flag);

for (k = shm; *k != '\0'; k++){
printf("%c",*k);
}
sleep(5);
printf("\nThe Read process of the Reader is complete!!!\n");
return 0;
}
