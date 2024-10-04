#ifndef ABSTRACT_H
#define ABSTRACT_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/shm.h>
#include <errno.h>
#define MAX_SIZE 6//5+1
static int num=1;
typedef enum { MUTEX, FULL, EMPTY } Type;
typedef struct
{
Type type;
int* value;
} Semaphore;
typedef struct
{
int* value;
int x;
} Buffer;
void wait(Semaphore semaphore)
{
switch (semaphore.type)
{
case MUTEX:
while (*semaphore.value == 0);
if (*semaphore.value)
{
*semaphore.value = 0;
return;
}
*semaphore.value = 1;
return;
case FULL:
if (*semaphore.value > 0)
(*semaphore.value)--;
case EMPTY:
if (*semaphore.value > 0)

(*semaphore.value)--;
}
}
void signal(Semaphore semaphore)
{
switch (semaphore.type)
{
case MUTEX:
if (*semaphore.value)
{
*semaphore.value = 0;
return;
}
*semaphore.value = 1;
return;
case FULL:
if (*semaphore.value + 1 < MAX_SIZE)
(*semaphore.value)++;
case EMPTY:
if (*semaphore.value + 1 < MAX_SIZE)
(*semaphore.value)++;
}
}
void insert_into(Buffer buffer, int value)
{
if (buffer.x == MAX_SIZE)
return ;
buffer.value[buffer.x++] = value;
}
int value_from(Buffer buffer)
{
if (buffer.x < 0)
return -1;
return buffer.value[buffer.x--];
}

Semaphore mutex = {MUTEX, NULL}, full = {FULL, NULL}, empty = {EMPTY, NULL};

Buffer buffer = {NULL, 0};
int producer()
{
if (*empty.value == 0)
{
printf("Buffer is full!Can't Produce!\n\n");
return -1;
}
printf("Cooking samosas :)\n");
wait(empty);
wait(mutex);
#pragma omp critical
printf("1 Samosa is ready!\n\n");
insert_into(buffer,num);
num++;
signal(mutex);
signal(full);
return 0;
}
int consumer()
{
if (*full.value == 0)
{
printf("Buffer is empty!Can't consume\n\n");
return -1;
}
wait(full);
wait(mutex);
#pragma omp critical
int x = value_from(buffer);
printf("1 Samosa was eaten!\n\n");
signal(mutex);
signal(empty);
return 0;
}
void init_shm()
{
int id;
void* pointer;

size_t size = MAX_SIZE * sizeof(int);
if ((id = shmget((key_t)0x1234, size, 0666)) == -1 && errno == ENOENT)
{ id = shmget((key_t)0x1234, size, IPC_CREAT | 0666);}
pointer = shmat(id, NULL, 0);
if (pointer == (void*)-1)
{
perror("SHM error!");
return;
}
buffer.value = (int*)pointer;
mutex.value = (int*)(pointer + MAX_SIZE * sizeof(int));
full.value = (int*)(mutex.value + sizeof(int));
empty.value = (int*)(full.value + sizeof(int));
*mutex.value = 1;
*full.value = 0;
*empty.value = MAX_SIZE;
}
void close_shm()
{
shmdt((void*)buffer.value);
}
#endif
