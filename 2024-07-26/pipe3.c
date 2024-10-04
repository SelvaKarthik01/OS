#include <unistd.h>
#include <stdio.h>
int main() {
int pipe_fd[2];
char buffer[50];
// Create a pipe
if (pipe(pipe_fd) == -1) {
perror("Pipe creation failed");
return 1;
}
pid_t child_pid = fork();
if (child_pid == 0) {
// Child process (consumer)
close(pipe_fd[1]); // Close write end
read(pipe_fd[0], buffer, sizeof(buffer));
printf("Child received: %s\n", buffer);
close(pipe_fd[0]);
} else {
// Parent process (producer)
close(pipe_fd[0]); // Close read end
write(pipe_fd[1], "Hello, IPC!", sizeof("Hello, IPC!"));
close(pipe_fd[1]);
}
return 0;
}
