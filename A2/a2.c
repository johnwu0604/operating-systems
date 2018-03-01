/*
 ----------------- COMP 310/ECSE 427 Winter 2018 -----------------
 Dimitri Gallos
 Assignment 2 skeleton

 -----------------------------------------------------------------
 I declare that the awesomeness below is a genuine piece of work
 and falls under the McGill code of conduct, to the best of my knowledge.
 -----------------------------------------------------------------
 */

//Please enter your name and McGill ID below
//Name: John Wu
//McGill ID: 260612056

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <semaphore.h>

#define BUFFER_SIZE 100
#define TIME_LIMIT 10

typedef struct
{
    int buf[BUFFER_SIZE];   /* shared var */
    int in;               /* buf[in%BUFFER_SIZE] is the first empty slot */
    int out;              /* buf[out%BUFFER_SIZE] is the first full slot */
    sem_t full;           /* keep track of the number of full spots */
    sem_t empty;          /* keep track of the number of empty spots */

    // use correct type here
    pthread_mutex_t mutex;          /* enforce mutual exclusion to shared data */
} sbuf_t;

sbuf_t shared;

unsigned concatenate(unsigned x, unsigned y) {
    unsigned pow = 10;
    while(y >= pow)
        pow *= 10;
    return x * pow + y;
}

/*Producer Function: Simulates an Airplane arriving and dumping 5-10 passengers to the taxi platform */
void *FnAirplane(void *arg)
{
    int t = 1;
    while(t <= TIME_LIMIT) {
        int item, airplane_id;

        airplane_id = (intptr_t)arg;

        int passengers = rand() % 6 + 5;
        printf("Airplane %d arrives with %d passengers \n", airplane_id, passengers );

        /* If another thread uses the buffer, wait */
        pthread_mutex_lock(&shared.mutex);
        for (int i=0; i<passengers; i++) {
            /* If there are no empty slots, rest of passengers take other transport */
            if (!&shared.empty) {
                printf("Taxi Line Full");
                break;
            }
            int time_id = t == 10 ? 0: t;
            printf("Passenger %d%03d%03d of airplane %d arrives to platform \n", time_id, airplane_id, i, airplane_id);
            shared.buf[shared.in] = i;
            shared.in = (shared.in+1)%BUFFER_SIZE;
            fflush(stdout);
        }
        /* Release the buffer */
        pthread_mutex_unlock(&shared.mutex);
        /* Increment the number of full slots */
        sem_post(&shared.full);
        usleep(1000*1000);
        t++;
    }
    return NULL;
}

/* Consumer Function: simulates a taxi that takes n time to take a passenger home and come back to the airport */
void *FnTaxi(void *arg)
{
    int i, item, index;

    index = (intptr_t)arg;

    sem_wait(&shared.full);
    pthread_mutex_lock(&shared.mutex);
    item=1;
    item=shared.buf[shared.out];
    shared.out = (shared.out+1)%BUFFER_SIZE;
    fflush(stdout);
    /* Release the buffer */
    pthread_mutex_unlock(&shared.mutex);
    /* Increment the number of full slots */
    sem_post(&shared.empty);

    /* Interleave  producer and consumer execution */
    usleep(500*1000);
    return NULL;
}

int main(int argc, char *argv[])
{

    int num_airplanes;
    int num_taxis;

    num_airplanes=atoi(argv[1]);
    num_taxis=atoi(argv[2]);

    printf("You entered: %d airplanes per hour\n",num_airplanes);
    printf("You entered: %d taxis\n", num_taxis);

    // create arrays of integer pointers to ids for taxi / airplane threads
    int *taxi_ids[num_taxis];
    int *airplane_ids[num_airplanes];

    // initialize semaphores
    sem_init(&shared.full, 0, 0);
    sem_init(&shared.empty, 0, BUFFER_SIZE);
    pthread_mutex_init(&shared.mutex, NULL);

    // instantiate producers
    for (int i=0; i<num_airplanes; i++)
    {
        /* Create a new producer */
        pthread_t airplane_thread;
        printf("Creating airplane thread %d \n", i);
        pthread_create(&airplane_thread, NULL, FnAirplane, (void*)(intptr_t)i);
    }
    /*create a new Consumer*/
    for(int i=0; i<num_taxis; i++)
    {
        pthread_t taxi_thread;
        pthread_create(&taxi_thread, NULL, FnTaxi, (void*)(intptr_t)i);
    }

    pthread_exit(NULL);
}