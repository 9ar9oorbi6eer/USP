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
        return -1;  // Indicates an error
    }

    if (pid == 0) 
    {
        close(pipefd[1]); // Child does not need to write to the pipe
        // Perform child-specific tasks here
        // It's important to exit after completing to avoid running parent code
        exit(0);
    } else 
    {
        close(pipefd[0]); // Parent does not need to read from the pipe
    }
    return pid; // Return the PID of the child to the parent
}
