//
// Example from: http://www.amparo.net/ce155/sem-ex.c
//
// Adapted using some code from Downey's book on semaphores
//
// Compilation:
//
//       g++ serialization.cpp -lpthread -o serial -lm
//
//
//

#include <unistd.h>     /* Symbolic Constants */
#include <sys/types.h>  /* Primitive System Data Types */
#include <errno.h>      /* Errors */
#include <stdio.h>      /* Input/Output */
#include <stdlib.h>     /* General Utilities */
#include <pthread.h>    /* POSIX Threads */
#include <string.h>     /* String handling */
#include <semaphore.h>  /* Semaphore */
#include <iostream>
using namespace std;





/*
 This wrapper class for semaphore.h functions is from:
 http://stackoverflow.com/questions/2899604/using-sem-t-in-a-qt-project
 */
class Semaphore {
public:
    // Constructor
    Semaphore(int initialValue)
    {
        sem_init(&mSemaphore, 0, initialValue);
    }
    // Destructor
    ~Semaphore()
    {
        sem_destroy(&mSemaphore); /* destroy semaphore */
    }
    
    // wait
    void wait()
    {
        sem_wait(&mSemaphore);
    }
    // signal
    void signal()
    {
        sem_post(&mSemaphore);
    }
    
    
private:
    sem_t mSemaphore;
};



/* prototype for thread routine */
void *threadB ( void *ptr );
void *threadA ( void *ptr );


/* global vars */
/* semaphores are declared global so they can be accessed
 in main() and in thread routine. */
Semaphore B_Done(0);


int main()
{
    int i[3];
    pthread_t thread_a;
    pthread_t thread_b;
    
    i[0] = 0; /* argument to threads */
    i[1] = 1;
    
    
    /* Note: you can check if thread has been
     successfully created by checking return value of
     pthread_create */
    pthread_create (&thread_a, NULL, threadA, (void *) &i[0]);
    pthread_create (&thread_b, NULL, threadB, (void *) &i[1]);
    
    // Join threads
    (void) pthread_join(thread_a, NULL);
    (void) pthread_join(thread_b, NULL);
    
    /* exit */
    exit(0);
    
} /* main() */


void *threadA ( void *ptr )
{
    int x;
    x = *((int *) ptr);
    
    B_Done.wait();

    printf("Thread %d: Statement A: Must run after Statement B. \n", x);
    fflush(stdout);
    
    B_Done.signal();
    
    
    pthread_exit(0); /* exit thread */
}

void *threadB ( void *ptr )
{
    int x;
    x = *((int *) ptr);
    printf("Thread %d: Statement B: Must run before Statement A. \n", x);
    fflush(stdout);
    
    B_Done.signal();
    
    
    pthread_exit(0); /* exit thread */
}
