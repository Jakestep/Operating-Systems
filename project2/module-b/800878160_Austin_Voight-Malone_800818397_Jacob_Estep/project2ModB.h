/**
 * Jacob Estep -- 800818397
 * Austin Voight-Malone -- 800878160
 */


#ifndef PROJECT2MODB
#define PROJECT2MODB

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h> /*for pthread library*/

int threshold;

/*=============================ToDo 1: =======================
Complete body of threshold function.
Hint: google how to pass multiple parameters to a function that is executed by a thread created thorugh pthread_create().
NOTE: A thread running this function must exit (i.e., use pthread_exit()) after exceuting the function body.*/

struct arguments {
    char *image;   // the whole image (shared by every thread)
    int start;     // first pixel index this thread handles
    int end;       // one past the last pixel index this thread handles
};

void threshold_operation(struct arguments *args){
    unsigned char *pixels = (unsigned char *)args->image;

    for (int i = args->start; i < args->end; i++) {
        if (pixels[i] >= threshold)
            pixels[i] = 255;
        else
            pixels[i] = 0;
    }

    pthread_exit(NULL);
}

#endif