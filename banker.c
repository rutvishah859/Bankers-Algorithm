#include <stddef.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stdbool.h> 
#include <unistd.h> 
#include <signal.h> 
#include <sys/types.h>
#include <sys/wait.h> 
#include <pthread.h>

/* these may be any values >= 0 */ 
#define NUMBER_OF_CUSTOMERS 5 
#define NUMBER_OF_RESOURCES 3 
 
/* the available amount of each resource */ 
int available[NUMBER_OF_RESOURCES]; 
 
/*the maximum demand of each customer */ 
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES]; 
 
/* the amount currently allocated to each customer */ 
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES]; 
 
/* the remaining need of each customer */ 
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

pthread_mutex_t mutex;

// return 0 if successful (request has been granted), return -1 if unsuccessful
// request random numbers of resources
int request_resources(int customer_num, int request[NUMBER_OF_RESOURCES]) {
    for (int m = 0; m < NUMBER_OF_RESOURCES; m++) {
        if (request[m] <= need[customer_num][m]) {
            if (request[m] <= available[m]) {
                printf("request is %d need is %d and avalibility is %d\n", request[m], need[customer_num][m], available[m]);
                // pretend to allocate requested resources
                available[m] -= request[m];
                allocation[customer_num][m] += request[m];
                need[customer_num][m] -= request[m];
                printf("avalible is %d allocation is %d and need is %d\n", available[m], allocation[customer_num][m], need[customer_num][m]);
            }
            else {
                // process must wait for resource to be available (unsafe state) 
                return -1;
            }
        }
        else {
            // process has exceeded its maximun claim (unsafe state)
            return -1;
        }
    }
    return 0;
}

// return 0 if successful (request has been granted), return -1 if unsuccessful
// releases random numbers of resources
int release_resources(int customer_num, int release[NUMBER_OF_RESOURCES]) {
    for (int m = 0; m < NUMBER_OF_RESOURCES; m++) {
        printf("released %d is %d\n", m, release[m]);
        // release the allocation
        available[m] += release[m];
        allocation[customer_num][m] -= release[m];
        need[customer_num][m] += release[m];
        // if the release is invalid return unsuccessful
        if (allocation[customer_num][m] < 0 || need[customer_num][m] < 0) {
            return -1;
        }
    }
    return 0;
}

void *customer(void *customer_num) {
    int customer = *(int *)customer_num;
    int safe = -1;
    // array consisting of requests made to each resource
    int request[NUMBER_OF_RESOURCES];
    // printf("the customer num is %d\n", customer);
    // generate the request for each resoruce (should be less then the max demand for given process)
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        request[i] = rand() % (maximum[customer][i] + 1);
        printf("the max is %d and the request is %d\n", maximum[customer][i], request[i]);
    }

    pthread_mutex_lock(&mutex);
    /* Critical Section*/
    safe = request_resources(customer, request);
    printf("request safe: %d\n", safe);
    pthread_mutex_unlock(&mutex);

    if (safe == 0) {
        safe = -1;
        pthread_mutex_lock(&mutex);
        printf("release safe %d\n", release_resources(customer, request));
        pthread_mutex_unlock(&mutex);
    }
    else {
        printf("Unsuccessful\n");
    }
}
    
int main(int argc, char *argv[]) {
    pthread_t th[NUMBER_OF_CUSTOMERS];
    pthread_mutex_init(&mutex, NULL);

    if (argc != NUMBER_OF_RESOURCES + 1) {
        exit(EXIT_FAILURE);
    }

    // allocates the number of available resources
    for (int m = 0; m < NUMBER_OF_RESOURCES; m++) {
        available[m] = atoi(argv[m + 1]);
    }

    printf("----Max----\n");
    // allocates the maximum demand of each customer
    for (int n = 0; n < NUMBER_OF_CUSTOMERS; n++) {
        for (int m = 0; m < NUMBER_OF_RESOURCES; m++) {
            allocation[n][m] = 0;
            // choose a random resource
            maximum[n][m] = rand() % (available[m] + 1);
            need[n][m] = maximum[n][m] - allocation[n][m];
            printf("%3d", maximum[n][m]);
        }
        printf("\n");
    }

    // create n customer threads
    for (int n = 0; n < NUMBER_OF_CUSTOMERS; n++) {
        int *cust = malloc(sizeof(*cust));
        *cust = n;
        pthread_create(&th[n], NULL, &customer, cust);
    }

    // join n customer threads
    for (int n = 0; n < NUMBER_OF_CUSTOMERS; n++) {
        pthread_join(th[n], NULL);
    }

    pthread_mutex_destroy(&mutex);
    exit(EXIT_SUCCESS);
}