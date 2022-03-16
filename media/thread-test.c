#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

#ifndef NUM_THREADS
#define NUM_THREADS 8
#endif

void *printHello(void *threadid) {
    long tid;
    tid = (long)threadid;
    printf("Hello from thread %ld, pthread ID - %lu\n", tid, pthread_self());
    return NULL;
}


int main(int argc, char const *argv[]) {
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;

    for (t = 0; t < NUM_THREADS; t++) {
        rc = pthread_create(&threads[t], NULL, printHello, (void *)t);
        if (rc) {
            printf("ERORR; return code from pthread_create() is %d\n", rc);
            exit(EXIT_FAILURE);
        }
    }

    int ret;
    for (t = 0; t < NUM_THREADS; t++) {
        void *retval;
        ret = pthread_join(threads[t], &retval);
        if (retval == PTHREAD_CANCELED)
            printf("The thread was canceled - ");
        else
            printf("Returned value %d - ", (int)retval);
    }
    pthread_exit(NULL);
}