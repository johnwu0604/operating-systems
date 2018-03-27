#include <stdio.h>  //for printf and scanf
#include <stdlib.h> //for malloc

#define LOW 0
#define HIGH 199
#define START 53

//compare function for qsort
//you might have to sort the request array
//use the qsort function
// an argument to qsort function is a function that compares 2 quantities
//use this there.
int cmpfunc (const void * a, const void * b) {
    return ( *(int*)a - *(int*)b );
}

//function to swap 2 integers
void swap(int *a, int *b)
{
    if (*a != *b)
    {
        *a = (*a ^ *b);
        *b = (*a ^ *b);
        *a = (*a ^ *b);
        return;
    }
}

//Prints the sequence and the performance metric
void printSeqNPerformance(int *request, int numRequest)
{
    int i, last, acc = 0;
    last = START;
    printf("\n");
    printf("%d", START);
    for (i = 0; i < numRequest; i++)
    {
        printf(" -> %d", request[i]);
        acc += abs(last - request[i]);
        last = request[i];
    }
    printf("\nPerformance : %d\n", acc);
    return;
}

//access the disk location in FCFS
void accessFCFS(int *request, int numRequest)
{
    //simplest part of assignment
    printf("\n----------------\n");
    printf("FCFS :");
    printSeqNPerformance(request, numRequest);
    printf("----------------\n");
    return;
}

//access the disk location in SSTF
void accessSSTF(int *request, int numRequest)
{
    // sort requests using quicksort
    qsort(request, numRequest, sizeof(int), cmpfunc);
    // array to store final request processing order
    int *finalOrder = malloc(numRequest * sizeof(int));
    // left and right pointers to keep track of two indices on each side closet to track head
    int left = 0;
    int right = numRequest-1;
    // keep track of where current track head is
    int current = START;
    // keep track of next index to allocate in final order
    int index = 0;
    // find the closest request to the left of the start
    while (left < right && request[left+1] < START) {
        left++;
    }
    // find the closest request to the right of the start
    while (right > left && request[right-1] > START) {
        right--;
    }
    // compare left and right pointers to see which one is closer
    // assign the closer location to the final request
    // keep iterating pointers outward until the boundary indices
    while (left<right && left >= 0 && right <= numRequest-1) {////access the disk location in SCAN
        int leftDistance = current - request[left];
        int rightDistance = request[right] - current;
        if (leftDistance < rightDistance) {
            finalOrder[index] = request[left];
            current = request[left];
            left--;
        } else {
            finalOrder[index] = request[right];
            current = request[right];
            right++;
        }
        index++;
    }
    // make sure pointers don't equal each other
    if (left==right) {
        right++;
    }
    // add remaining values in order from right to left
    while (left >= 0) {
        finalOrder[index] = request[left];
        left--;
        index++;
    }
    // add remaining values in order from left to right
    while (right <= numRequest-1) {
        finalOrder[index] = request[right];
        right++;
        index++;
    }

    printf("\n----------------\n");
    printf("SSTF :");
    printSeqNPerformance(finalOrder, numRequest);
    printf("----------------\n");
    return;
}

//access the disk location in SCAN
void accessSCAN(int *request, int numRequest)
{

    // sort requests using quicksort
    qsort(request, numRequest, sizeof(int), cmpfunc);
    // array to store final request processing order
    int *finalOrder = malloc((numRequest+1) * sizeof(int));
    // assume head is moving in the increasing direction initially
    int start = 0;
    int current = 0;
    int index = 0;
    // find start index
    while (request[start] < START && start < numRequest) {
        start++;
    }
    current = start;
    // move in increasing direction
    while (current < numRequest) {
        finalOrder[index] = request[current];
        index++;
        current++;
    }
    // go back to start and move in opposite direction
    current = start-1;
    printf("\n----------------\n");
    printf("SCAN :");
    if (current >= 0) {
        finalOrder[index] = HIGH;
        index++;
        while (current >= 0) {
            finalOrder[index] = request[current];
            index++;
            current--;
        }
        printSeqNPerformance(finalOrder, numRequest+1);
    } else {
        printSeqNPerformance(finalOrder, numRequest);
    }
    printf("----------------\n");
    return;
}

//access the disk location in CSCAN
void accessCSCAN(int *request, int numRequest)
{
    // sort requests using quicksort
    qsort(request, numRequest, sizeof(int), cmpfunc);
    // array to store final request processing order
    int *finalOrder = malloc((numRequest+2) * sizeof(int));
    int current = 0;
    int index = 0;
    // find start index
    while (request[current] < START && current < numRequest) {
        current++;
    }
    while (current < numRequest) {
        finalOrder[index] = request[current];
        index++;
        current++;
    }
    printf("\n----------------\n");
    printf("CSCAN :");
    if (index == numRequest) {
        printSeqNPerformance(finalOrder, numRequest);
    } else {
        finalOrder[index] = HIGH;
        index++;
        finalOrder[index] = LOW;
        index++;
        current = 0;
        while (index <= numRequest+1) {
            finalOrder[index] = request[current];
            index++;
            current++;
        }
        printSeqNPerformance(finalOrder, numRequest+2);
    }
    printf("----------------\n");
    return;
}

//access the disk location in LOOK
void accessLOOK(int *request, int numRequest)
{
    // sort requests using quicksort
    qsort(request, numRequest, sizeof(int), cmpfunc);
    // array to store final request processing order
    int *finalOrder = malloc((numRequest+1) * sizeof(int));
    // assume head is moving in the increasing direction initially
    int start = 0;
    int current = 0;
    int index = 0;
    // find start index
    while (request[start] < START && start < numRequest) {
        start++;
    }
    current = start;
    // move in increasing direction
    while (current < numRequest) {
        finalOrder[index] = request[current];
        index++;
        current++;
    }
    // go back to start and move in opposite direction
    current = start-1;
    while (current >= LOW) {
        finalOrder[index] = request[current];
        index++;
        current--;
    }

    printf("\n----------------\n");
    printf("SCAN :");
    printSeqNPerformance(finalOrder, numRequest);
    printf("----------------\n");
    return;
}

//access the disk location in CLOOK
void accessCLOOK(int *request, int numRequest)
{
    // sort requests using quicksort
    qsort(request, numRequest, sizeof(int), cmpfunc);
    // array to store final request processing order
    int *finalOrder = malloc((numRequest+1) * sizeof(int));
    int current = 0;
    int index = 0;
    // find start index
    while (request[current] < START && current < numRequest) {
        current++;
    }
    while (current < numRequest) {
        finalOrder[index] = request[current];
        index++;
        current++;
    }
    printf("\n----------------\n");
    printf("CLOOK :");
    if (index == numRequest) {
        printSeqNPerformance(finalOrder, numRequest);
    } else {
        finalOrder[index] = LOW;
        index++;
        current = 0;
        while (index <= numRequest) {
            finalOrder[index] = request[current];
            index++;
            current++;
        }
        printSeqNPerformance(finalOrder, numRequest+1);
    }
    printf("----------------\n");
    return;
}

int main()
{
    int *request, numRequest, i,ans;

    //allocate memory to store requests
    printf("Enter the number of disk access requests : ");
    scanf("%d", &numRequest);
    request = malloc(numRequest * sizeof(int));

    printf("Enter the requests ranging between %d and %d\n", LOW, HIGH);
    for (i = 0; i < numRequest; i++)
    {
        scanf("%d", &request[i]);
    }

    printf("\nSelect the policy : \n");
    printf("----------------\n");
    printf("1\t FCFS\n");
    printf("2\t SSTF\n");
    printf("3\t SCAN\n");
    printf("4\t CSCAN\n");
    printf("5\t LOOK\n");
    printf("6\t CLOOK\n");
    printf("----------------\n");
    scanf("%d",&ans);

    switch (ans)
    {
        //access the disk location in FCFS
        case 1: accessFCFS(request, numRequest);
            break;

            //access the disk location in SSTF
        case 2: accessSSTF(request, numRequest);
            break;

            //access the disk location in SCAN
        case 3: accessSCAN(request, numRequest);
            break;

            //access the disk location in CSCAN
        case 4: accessCSCAN(request,numRequest);
            break;

            //access the disk location in LOOK
        case 5: accessLOOK(request,numRequest);
            break;

            //access the disk location in CLOOK
        case 6: accessCLOOK(request,numRequest);
            break;

        default:
            break;
    }
    return 0;
}