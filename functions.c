#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int setup_pipe(int pipefd[2]) 
{
    if (pipe(pipefd) == -1) 
    {
        perror("pipe");
        return 1;
    }
    return 0;
}

int create_process_fork(int pipefd[2], int readfd[2])  // Updated to handle two pipes
{
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        close(pipefd[0]); // Close both ends of the command pipe
        close(pipefd[1]);
        close(readfd[0]); // Close both ends of the result pipe
        close(readfd[1]);
        return -1;  // Indicates an error
    }

    if (pid == 0) 
    {
        // Child process
        close(pipefd[1]); // Child does not need to write to the command pipe
        close(readfd[0]); // Child does not need to read from the result pipe
    } else 
    {
        // Parent process
        close(pipefd[0]); // Parent does not need to read from the command pipe
        close(readfd[1]); // Parent does not need to write to the result pipe
    }
    return pid; // Return the PID of the child to the parent
}
