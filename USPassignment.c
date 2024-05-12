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
    int pipefd[2];  // Pipe for sending commands to the child
    int readfd[2];  // Pipe for receiving results from the child

    // Set up pipes for commands and results
    if (setup_pipe(pipefd) || setup_pipe(readfd)) 
    {
        perror("Failed to set up pipes");
        return 1; // Error occurred in pipe setup
    }

    // Create child process and handle both pipes
    pid_t pid = create_process_fork(pipefd, readfd);
    if (pid == -1) 
    {
        perror("Failed to create process fork");
        return 1; // Error occurred in process fork
    }

    if (pid == 0) 
    {
        // Child process: handle filenames and send results
        close(pipefd[1]); // Close unused write end of command pipe
        close(readfd[0]); // Close unused read end of result pipe
        char buffer[256];
        int count;
        read(pipefd[0], &count, sizeof(count));
        for (int i = 0; i < count; i++) {
            read(pipefd[0], buffer, 256);
            // Simulate processing and generate a result
            sprintf(buffer, "%s: Processed", buffer);
            write(readfd[1], buffer, strlen(buffer) + 1);
        }
        close(pipefd[0]);
        close(readfd[1]);
        exit(0);
    }
    else 
    {
        // Parent process: send filenames and read results
        close(pipefd[0]); // Close unused read end of command pipe
        close(readfd[1]); // Close unused write end of result pipe
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
        // Send the count and file names to the child
        write(pipefd[1], &file_count, sizeof(file_count));
        for (int i = 0; i < file_count; i++) {
            write(pipefd[1], files[i], strlen(files[i]) + 1);
            free(files[i]);  // Free memory as soon as it's no longer needed
        }
        close(pipefd[1]);  // Close write end after sending

        // Read results from child and write to file
        FILE *fp = fopen("result.txt", "w");
        if (fp == NULL) {
            perror("Failed to open result.txt");
            return 1;
        }
        char result[256];
        while (read(readfd[0], result, 256) > 0) {
            fprintf(fp, "%s\n", result);
        }
        fclose(fp);
        close(readfd[0]);

        // Wait for the child process to complete
        int status;
        waitpid(pid, &status, 0);
    }

    return 0;
}
