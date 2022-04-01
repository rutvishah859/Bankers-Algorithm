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

void *customer() {

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
    // allocates the maximum demand of each customer
    for (int n = 0; n < NUMBER_OF_CUSTOMERS; n++) {
        for (int m = 0; m < NUMBER_OF_RESOURCES; m++) {
            allocation[n][m] = 0;
            // choose a random resource
            maximum[n][m] = rand() % (available[m] + 1);
            need[n][m] = maximum[n][m];
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
        pthread_join(th[n], 0);
    }

    pthread_mutex_destroy(&mutex);
    exit(EXIT_SUCCESS);
}