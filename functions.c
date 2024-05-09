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


int create_process_fork(int pipefd[2]) 
{
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        close(pipefd[0]); // Close the read end
        close(pipefd[1]); // Close the write end
        return 1;
    }

    if (pid == 0) 
    {
        // Child process
        close(pipefd[1]); // Child does not need to write to the pipe
    } else 
    {
        // Parent process
        close(pipefd[0]); // Parent does not need to read from the pipe
    }
}