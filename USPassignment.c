#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "functions.h"

#define MAX_FILES 100

int main() 
{
    DIR *d;
    struct dirent *dir;
    char *files[MAX_FILES];
    int file_count = 0;
    int pipefd[2];

    // Set up the pipe
    if (setup_pipe(pipefd)) 
    {
        return 1; // Error occurred in pipe setup
    }

    // Create child process
    pid_t pid = create_process_fork(pipefd); // Ensure this returns the PID
    if (pid == -1) 
    {
        return 1; // Error occurred in process fork
    }

    if (pid == 0) 
    {
        // Child process: handle other responsibilities
        // Example: Child could be reading from pipe or handling other tasks
        // It's crucial to exit after completing the tasks to prevent executing parent's code
        exit(0);
    }
    else 
    {
        // Parent process: retrieve the list of .usp files
        d = opendir(".");
        if (d)
        {
            while ((dir = readdir(d)) != NULL)
            {
                if (strstr(dir->d_name, ".usp"))
                {
                    files[file_count] = malloc(strlen(dir->d_name) + 1);
                    strcpy(files[file_count], dir->d_name);
                    file_count++;
                    if (file_count >= MAX_FILES) break;
                }
            }
            closedir(d);
        }

        // Display file names for debugging
        for (int i = 0; i < file_count; i++)
        {
            printf("File %d: %s\n", i + 1, files[i]);
        }

        // Free allocated memory and close resources
        for (int i = 0; i < file_count; i++)
        {
            free(files[i]);
        }

        // Wait for child process to complete (if needed)
        int status;
        waitpid(pid, &status, 0);
    }

    return 0;
}
