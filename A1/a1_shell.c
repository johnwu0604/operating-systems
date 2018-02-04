/*
----------------- COMP 310/ECSE 427 Winter 2018 -----------------
I declare that the awesomeness below is a genuine piece of work
and falls under the McGill code of conduct, to the best of my knowledge.
-----------------------------------------------------------------
*/

//Please enter your name and McGill ID below
//Name: John Wu
//McGill ID: 260612056

//all the header files you would require
#include <stdio.h>  //for standard IO
#include <unistd.h> //for execvp/dup/dup2
#include <string.h> //for string manipulation
#include <stdlib.h> //for fork
#include <ctype.h>  //for character type check (isnum,isalpha,isupper etc)
#include <sys/wait.h>//for waitpid
#include <fcntl.h>  //open function to open a file. type "man 2 open" in terminal
#include <time.h>   //to handle time

//pointer to Linked list head
struct node *head_job = NULL;

//pointer to
struct node *current_job = NULL;

//global variable used to store process id of process
//that has been just created
//can be used while adding a job to linked list
pid_t process_id;

//flag variable to check if redirection of output is required
int isred = 0;

/**
 * Structure of a single node
 */
struct node
{
    int number;        //the job number
    int pid;           //the process id of the process
    char *cmd;         //string to store the command name
    time_t spawn;      //time to store the time it was spawned
    struct node *next; //when another process is called you add to the end of the linked list
};

/**
 * Add a job to the linked list
 */
void addToJobList(char *args[])
{
    //allocate memory for the new job
    struct node *job = malloc(sizeof(struct node));
    //If the job list is empty, create a new head
    if (head_job == NULL)
    {
        //init the job number with 1

        //set its pid from the global variable process_id

        //cmd can be set to arg[0]

        //set the job->next to point to NULL.

        //set the job->spawn using time function
        job->spawn = (unsigned int)time(NULL);
        //set head_job to be the job

        //set current_job to be head_job

    }

        //Otherwise create a new job node and link the current node to it
    else
    {
        //point current_job to head_job

        //traverse the linked list to reach the last job




        //init all values of the job like above num,pid,cmd.spawn


        //make next of current_job point to job

        //make job to be current_job

        //set the next of job to be NULL

    }
}

/**
 * Function to refresh job list.
 * Run through jobs in in linked list and check.
 * If they are done executing, then remove it.
 */
void refreshJobList()
{
    //pointer require to perform operation
    //on linked list
    struct node *current_job;
    struct node *prev_job;

    //variable to store returned pid
    pid_t ret_pid;

    //perform init for pointers
    current_job = head_job;
    prev_job = head_job;

    //traverse through the linked list
    while (current_job != NULL)
    {
        //use waitpid to init ret_pid variable
        ret_pid = waitpid(current_job->pid, NULL, WNOHANG);
        if (ret_pid == 0)
        {
            // process has ended, delete node
            prev_job->next = current_job->next;
            current_job = current_job->next;
        }
        else
        {
            // process still running, keep node
            prev_job = current_job;
            current_job = current_job->next;
        }
    }
    return;
}


/**
 * Function that lists all the jobs
 */
void listAllJobs()
{
    struct node *current_job;
    int ret_pid;

    //refresh the linked list
    refreshJobList();

    //init current_job with head_job
    current_job = head_job;

    //heading row print only once.
    printf("\nID\tPID\tCmd\tstatus\tspawn-time\n");

    //traverse the linked list and print using the following statement for each job
    while(current_job != NULL) {
        //ret_pid = waitpid(current_job->pid, NULL, WNOHANG);
//        if (ret_pid == 0) {
//            printf("%d\t%d\t%s\tRUNNING\t%s\n", current_job->number, current_job->pid, current_job->cmd, ctime(&(current_job->spawn)));
//        }
        printf("%d\t%d\t%s\tRUNNING\t%s\n", current_job->number, current_job->pid, current_job->cmd, ctime(&(current_job->spawn)));
        current_job = current_job->next;
    }
    return;
}

/**
 * Wait till the linked list is empty.
 * You would have to look for a place where you
 * would call this function. Do not modify this function
 * @param nice
 * @param bg
 */
void waitForEmptyLL(int nice, int bg)
{
    if (nice == 1 && bg == 0)
    {
        while (head_job != NULL)
        {
            sleep(1);
            refreshJobList();
        }
    }
    return;
}

/**
 * Function to perform word count
 *
 * @param filename
 * @param flag
 * @return
 */
int wordCount(char *filename,char* flag)
{
    int cnt;
    //if flag is l
    //count the number of lines in the file
    //set it in cnt

    //if flag is w
    //count the number of words in the file
    //set it in cnt

    return cnt;
}

/**
 * Function to augment waiting times for a processs.
 * Do not modify this function.
 */
void performAugmentedWait()
{
    int w, rem;
    time_t now;
    srand((unsigned int)(time(&now)));
    w = rand() % 15;
    printf("sleeping for %d\n", w);
    rem = sleep(w);
    return;
}

/**
 * Simulates running process to foreground by
 * making the parent process wait for a
 * particular process id.
 *
 * @param jobnc
 * @return
 */
int waitforjob(char *jobnc)
{
    struct node *trv;
    int jobn = (*jobnc) - '0';
    trv = head_job;
    //traverse through linked list and find the corresponding job
    while (trv != NULL) {
        if (trv->number = jobn) {
            break;
        }
        trv = trv->next;
    }
    waitpid(trv->pid,NULL,0);
    return 0;
}

/**
 * Splits whatever the user enters and sets the background/nice flag variable
 * and returns the number of tokens processed.
 * Do not modify this function.
 *
 * @param prompt
 * @param args
 * @param background
 * @param nice
 * @return
 */
int getcmd(char *prompt, char *args[], int *background, int *nice)
{
    int length, i = 0;
    char *token, *loc;
    char *line = NULL;
    size_t linecap = 0;
    printf("%s", prompt);
    length = getline(&line, &linecap, stdin);
    if (length <= 0)
    {
        exit(-1);
    }
    // Check if background is specified..
    if ((loc = index(line, '&')) != NULL)
    {
        *background = 1;
        *loc = ' ';
    }
    else
        *background = 0;
    while ((token = strsep(&line, " \t\n")) != NULL)
    {
        for (int j = 0; j < strlen(token); j++)
            if (token[j] <= 32)
                token[j] = '\0';
        if (strlen(token) > 0)
        {
            if (!strcmp("nice", token))
            {
                *nice = 1;
            }
            else
            {
                args[i++] = token;
            }
        }
    }
    return i;
}

/**
 * This initializes the args to all null.
 * Do not modify.
 * @param args
 */
void initialize(char *args[])
{
    for (int i = 0; i < 20; i++)
    {
        args[i] = NULL;
    }
    return;
}

int main(void)
{
    //args is a array of character pointers
    //where each pointer points to a string
    //which may be command , flag or filename
    char *args[20];

    //flag variables for background, status and nice
    //bg set to 1 if the command is to executed in background
    //nice set to 1 if the command is nice
    //status
    int bg, status, nice;

    //variable to store the process id.
    pid_t pid;

    //variable to store the file des
    //helpful in output redirection
    int fd1, fd2;

    //your terminal executes endlessly unless
    //exit command is received
    while (1)
    {
        //init background to zero
        bg = 0;
        //init nice to zero
        nice = 0;
        //init args to null
        initialize(args);
        //get the user input command
        int cnt = getcmd("\n>> ", args, &bg, &nice);
        //keep asking unless the user enters something
        while (!(cnt >= 1))
            cnt = getcmd("\n>> ", args, &bg, &nice);

        //use the if-else ladder to handle built-in commands
        //built in commands don't need redirection
        //also no need to add them to jobs linked list
        //as they always run in foreground
        if (!strcmp("jobs", args[0]))
        {
            //call the listalljobs function
            listAllJobs();
        }
        else if (!strcmp("exit", args[0]))
        {
            //exit the execution of endless while loop
            exit(0);
        }
        else if (!strcmp("fg", args[0]))
        {
            //bring a background process to foreground
            waitforjob(args[1]);
        }
        else if (!strcmp("cd", args[0]))
        {
            // if no destination directory given
            // change to home directory
            if (args[1] == NULL) {
                args[1] = "/home";
            }

            //if given directory does not exist
            //print directory does not exit

            //if everthing is fine
            //change to destination directory
            chdir(args[1]);
        }
        else if (!strcmp("ls", args[0]))
        {
            int status;
            char *argv[2];

            argv[0] = "/bin/ls";        // first arg is the full path to the executable
            argv[1] = NULL;             // list of args must be NULL terminated

            if ( fork() == 0 )
                execv( argv[0], argv ); // child: call execv with the path and the args
            else
                wait( &status );        // parent: wait for the child (not really necessary)
        }
//        else if (!strcmp("pwd", args[0]))
//        {
//            //use getcwd and print the current working directory
//
//        }
//        else if(!strcmp("wc",args[0]))
//        {
//            //call the word count function
//            wordCount(args[2],args[1]);
//        }
//        else
//        {
//            //Now handle the executable commands here
//            /* the steps can be..:
//            (1) fork a child process using fork()
//            (2) the child process will invoke execvp()
//            (3) if background is not specified, the parent will wait,
//                otherwise parent starts the next command... */
//
//
//            //hint : samosas are nice but often there
//            //is a long waiting line for it.
//
//            //create a child
//            pid = fork();
//
//            //to check if it is parent
//            if (pid > 0)
//            {
//                //we are inside parent
//                //printf("inside the parent\n");
//                if (bg == 0)
//                {
//                    //FOREGROUND
//                    // waitpid with proper argument required
//                }
//                else
//                {
//                    //BACKGROUND
//                    process_id = pid;
//                    addToJobList(args);
//                    // waitpid with proper argument required
//                }
//            }
//            else
//            {
//                // we are inside the child
//
//                //introducing augmented delay
//                performAugmentedWait();
//
//                //check for redirection
//                //now you know what does args store
//                //check if args has ">"
//                //if yes set isred to 1
//                //else set isred to 0
//
//                //if redirection is enabled
//                if (isred == 1)
//                {
//                    //open file and change output from stdout to that
//                    //make sure you use all the read write exec authorisation flags
//                    //while you use open (man 2 open) to open file
//
//                    //set ">" and redirected filename to NULL
//                    args[i] = NULL;
//                    args[i + 1] = NULL;
//
//                    //run your command
//                    execvp(args[0], args);
//
//                    //restore to stdout
//                    fflush(stdout);
//                }
//                else
//                {
//                    //simply execute the command.
//                    execvp(args[0], args);
//                }
//            }
//        }
    }

    return 0;
}