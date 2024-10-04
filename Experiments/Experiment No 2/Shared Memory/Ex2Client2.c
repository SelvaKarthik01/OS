#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main() {
    key_t key = ftok("shmfile", 65); // Generate key
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT); // Access shared memory
    char *str = (char *)shmat(shmid, (void *)0, 0); // Attach to shared memory

    printf("Client 2: Data in uppercase:\n");
    for (int i = 0; str[i] != '\0'; i++) {
        printf("%c", toupper(str[i]));
    }

    shmdt(str); // Detach from shared memory
    return 0;
}

