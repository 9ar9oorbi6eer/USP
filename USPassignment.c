// pipe: a pipe allows one program to send infromation to another program
// fork: cloning, when you call fork, it makes an exact copy of itself, so you have two identical programs running at the same time
// open: just like opening a book, it is used to open files or resources, once you open a file you can read from it or write to it
// close: when your done with a file you close it
// read: lets you read from a file or resource into memory.
// write: lets you write information from memory into a file or resource
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "functions.h"

int main() 
{
    int pipefd[2];
    if (setup_pipe(pipefd)) 
    {
        return 1; // Error occurred in pipe setup
    }
    if (create_process_fork(pipefd)) 
    {
        return 1; // Error occurred in process fork
    }
    return 0;
}