#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    printf("First : Print to stdout \n");

    // save copy of stdout to revert back to afterwards
    int stdout_copy = dup(1);

    // Redirect output to redirect_out.txt
    int file=open("redirect_out.txt", O_CREAT | O_RDWR | O_APPEND,
                  S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    dup2(file,1);
    close(file);
    printf("Second : Print to redirect_out.txt \n");

    // Direct output back to stdout
    dup2(stdout_copy, 1);
    printf("Third : Print to stdout \n");

    return 0;
}