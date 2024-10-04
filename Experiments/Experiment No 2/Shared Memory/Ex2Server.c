#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main() {
    key_t key = ftok("shmfile", 65); // Generate key
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT); // Create shared memory
    char *str = (char *)shmat(shmid, (void *)0, 0); // Attach to shared memory

    printf("Enter multiple lines (type 'end' to finish):\n");
    char buffer[1024];
    str[0] = '\0'; // Initialize shared memory with an empty string

    while (1) {
        fgets(buffer, sizeof(buffer), stdin);
        if (strncmp(buffer, "end", 3) == 0)
            break;
        strcat(str, buffer); // Append the input line to the shared memory
    }

    printf("Data written in memory:\n%s", str);

    shmdt(str); // Detach from shared memory
    return 0;
}

