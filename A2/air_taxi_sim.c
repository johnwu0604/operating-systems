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
#include <sys/time.h>

#define BUFFER_SIZE 100
#define TIME_LIMIT 10

/**
 * The structure for our semaphore
 */
typedef struct
{
    int buf[BUFFER_SIZE];   /* shared var */
    int in;               /* buf[in%BUFFER_SIZE] is the first empty slot */
    int out;              /* buf[out%BUFFER_SIZE] is the first full slot */
    sem_t full;           /* keep track of the number of full spots */
    sem_t empty;          /* keep track of the number of empty spots */
    pthread_mutex_t mutex;          /* enforce mutual exclusion to shared data */
} sbuf_t;

/**
 * Create the shared resource
 */
sbuf_t shared;

/**
 * A method which outputs the passenger ID by following the required format
 *
 * @param airplane_id
 * @param passenger_id
 * @param time_id
 * @return
 */
int getPassengerId(int airplane_id, int passenger_id, int time_id) {
    char time_code[1];
    char airplane_code[3];
    char passenger_code[3];
    snprintf(time_code, 10, "%d", time_id);
    snprintf(airplane_code, 10, "%03d", airplane_id);
    snprintf(passenger_code, 10, "%03d", passenger_id);

    char id[7];
    id[0] = time_code[0];
    id[1] = airplane_code[0];
    id[2] = airplane_code[1];
    id[3] = airplane_code[2];
    id[4] = passenger_code[0];
    id[5] = passenger_code[1];
    id[6] = passenger_code[2];
    return atoi(id);
}

/**
 * Producer Function: Simulates an Airplane arriving and dumping 5-10 passengers to the taxi platform
 *
 * @param arg
 * @return
 */
void *FnAirplane(void *arg)
{
    // set the time initially to first hour
    int t = 1;
    // have planes continuously arriving every hour until the time limit
    while(t <= TIME_LIMIT) {
        // airplane id
        int airplane_id;
        // random number of passengers from 5-10
        int passengers = rand() % 6 + 5;

        // lock the shared resource
        pthread_mutex_lock(&shared.mutex);

        airplane_id = (intptr_t)arg;
        printf("Airplane %d arrives with %d passengers \n", airplane_id, passengers );

        for (int i=0; i<passengers; i++) {
            int open_slots;
            sem_getvalue(&shared.empty, &open_slots); // find number of open slots in the queue
            // if there are no empty slots, rest of passengers take other transport
            if (open_slots == 0) {
                printf("Platform is full: Rest of passengers of plane %d take the bus\n", airplane_id);
                i = passengers;
                break;
            } else {
                // if resource is being used, wait
                sem_wait(&shared.empty);
                int time_id = t == 10 ? 0: t;
                int passenger_id = getPassengerId(airplane_id, i, time_id);
                printf("Passenger %07d of airplane %d arrives to platform \n", passenger_id, airplane_id);
                // add passenger to end of queue and iterate the tail to next slot in buffer
                shared.buf[shared.in] = passenger_id;
                shared.in = (shared.in+1)%BUFFER_SIZE;
                fflush(stdout);
                // increment the number of full slots
                sem_post(&shared.full);
            }
        }
        // release the buffer
        pthread_mutex_unlock(&shared.mutex);
        // sleep for one second (one hour)
        usleep(1000*1000);
        t++;
    }
    return NULL;
}

/**
 * Consumer Function: simulates a taxi that takes n time to take a passenger home and come back to the airport
 *
 * @param arg
 * @return
 */
void *FnTaxi(void *arg)
{
    // store the taxi id and passenger id
    int taxi_id, passenger;

    // taxi continues in a loop until no more planes are planning to arrive
    while (1) {
        taxi_id = (intptr_t)arg;
        printf("Taxi driver %d arrives\n", taxi_id);

        // check if there is anyone in the queue
        int full_slots, iterator = 0;
        sem_getvalue(&shared.full, &full_slots);
        // if the queue is empty, taxi driver repeatedly checks for passengers until someone shows up or timeout
        if (full_slots == 0) {
            printf("Taxi driver %d waits for passengers to enter the platform\n", taxi_id);
            while (iterator < 100*TIME_LIMIT) {
                sem_getvalue(&shared.full, &full_slots);
                if (full_slots == 0) {
                    // sleep for 10 ms
                    iterator++;
                    usleep(10*1000);
                } else {
                    iterator = TIME_LIMIT*10000;
                }
            }
        }
        if (iterator == 100*TIME_LIMIT) {
            printf("Taxi driver %d leaves (no more planes)\n", taxi_id);
            break;
        }
        sem_wait(&shared.full);
        // lock shared resource
        pthread_mutex_lock(&shared.mutex);
        // take the next passenger off the queue
        passenger = shared.buf[shared.out];
        // take passenger off queue and next full slot in buffer becomes front of queue
        printf("Taxi driver %d picked up client %07d from the platform\n", taxi_id, passenger);
        shared.out = (shared.out+1)%BUFFER_SIZE;
        fflush(stdout);
        // decrement the number of empty slots
        sem_post(&shared.empty);
        // Release the buffer
        pthread_mutex_unlock(&shared.mutex);
        // Time delay for taxi to return which is anywhere from 10 to 30 min
        int time_delay = rand() % 11 + 30;
        float fractional_time = time_delay / 60.0;
        usleep(0.5*1000*1000);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    // number of airplanes and taxis from user arguments
    int num_airplanes;
    int num_taxis;
    num_airplanes=atoi(argv[1]);
    num_taxis=atoi(argv[2]);

    // write the output to a text file
    char file_name[256];
    sprintf(file_name,"%d_airplanes_%d_taxis.txt",num_airplanes,num_taxis);
    printf("Simulating taxi usage... \nOutput can be found in %s file once finished. \n", file_name);
    close(1);
    fopen(file_name, "w");

    printf("You entered: %d airplanes per hour\n",num_airplanes);
    printf("You entered: %d taxis\n", num_taxis);

    // create arrays of integer pointers to ids for taxi / airplane threads
    int *taxi_ids[num_taxis];
    int *airplane_ids[num_airplanes];

    // initialize semaphores
    sem_init(&shared.full, 0, 0);
    sem_init(&shared.empty, 0, BUFFER_SIZE);
    pthread_mutex_init(&shared.mutex, NULL);

    // instantiate airplane threads
    pthread_t airplane_thread, taxi_thread;
    for (int i=0; i<num_airplanes; i++)
    {
        printf("Creating airplane thread %d \n", i);
        pthread_create(&airplane_thread, NULL, FnAirplane, (void*)(intptr_t)i);
    }

    // instantiate taxi threads
    for(int i=0; i<num_taxis; i++)
    {
        pthread_create(&taxi_thread, NULL, FnTaxi, (void*)(intptr_t)i);
    }

    // exit thread
    pthread_exit(NULL);
    exit(1);
}