/*
----------------- COMP 310/ECSE 427 Winter 2018 -----------------
I declare that the awesomeness below is a genuine piece of work
and falls under the McGill code of conduct, to the best of my knowledge.
-----------------------------------------------------------------
*/

//Please enter your name and McGill ID below
//Name: John Wu
//McGill ID: 260612056

#include <stdio.h>
#include <unistd.h>

int main() {
    // create the pipe
    int pipefd[2];
    pipe(pipefd);
    // command arguments
    char *argv[] = { "ls", 0 };

    if (fork() == 0) {
        // Child process execution
        close(pipefd[0]); // close reading end of the pipe
        dup2(pipefd[1], 1); // send stdout to the pipe
        close(pipefd[1]); // close the descriptor
        execvp(argv[0],argv); // execute command
    } else {
        // Parent process execution
        char buffer[1024];
        close(pipefd[1]); // close the write end of the pipe
        read(pipefd[0], buffer, sizeof(buffer)); // read the data in the buffer
        printf("%s", buffer); // print to piped output
    }
    return 0;
}