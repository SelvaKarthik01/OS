#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    int p[3][2]; // Pipes for three child processes
    pid_t pid[3]; // Process IDs for the three child processes
    char *words[3] = {"apple", "banana", "cherry"};
    int counts[3] = {0}; // Array to hold the counts from each child
    char buffer[100]; // Buffer to read lines from the file
    FILE *file;
    
    // Create pipes
    for (int i = 0; i < 3; i++) {
        if (pipe(p[i]) == -1) {
            printf("Pipe creation failed\n");
            return 1;
        }
    }

    // Create child processes
    for (int i = 0; i < 3; i++) {
        pid[i] = fork();
        
        if (pid[i] > 0) { // Parent process
            close(p[i][0]); // Close unused read end of the pipe in parent
        } else if (pid[i] == 0) { // Child process
            close(p[i][1]); // Close unused write end of the pipe in child
            int count = 0;

            // Open the file and count occurrences of the assigned word
            file = fopen("/home/selva/OS Programs/document.txt", "r");
            if (file == NULL) {
                printf("Failed to open file\n");
                return 1;
            }

            while (fgets(buffer, sizeof(buffer), file)) {
                char *ptr = buffer;
                while ((ptr = strstr(ptr, words[i])) != NULL) {
                    count++;
                    ptr += strlen(words[i]);
                }
            }
            fclose(file);

            // Write the count to the pipe
            write(p[i][0], &count, sizeof(int));
            close(p[i][0]); // Close read end after writing
            exit(0); // Terminate child process
        } else { // Fork failed
            printf("Fork failed\n");
            return 1;
        }
    }

    // Parent process: Read the counts from each child
    for (int i = 0; i < 3; i++) {
        wait(NULL); // Wait for each child to finish
        read(p[i][1], &counts[i], sizeof(int));
        close(p[i][1]); // Close write end after reading
        printf("The word '%s' occurred %d times\n", words[i], counts[i]);
    }

    return 0;
}

