#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <ctype.h>

int main() {
    key_t key = ftok("shmfile", 65); // Generate key
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT); // Access shared memory
    char *str = (char *)shmat(shmid, (void *)0, 0); // Attach to shared memory

    int line_count = 0, word_count = 0, char_count = 0;
    int in_word = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        char_count++;

        if (str[i] == '\n') {
            line_count++;
        }

        if (isspace(str[i])) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            word_count++;
        }
    }

    printf("Client 3: Summary of the messages:\n");
    printf("Lines: %d\n", line_count);
    printf("Words: %d\n", word_count);
    printf("Characters: %d\n", char_count);

    shmdt(str); // Detach from shared memory
    shmctl(shmid, IPC_RMID, NULL); // Destroy shared memory after use
    return 0;
}

