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