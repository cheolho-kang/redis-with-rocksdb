#pragma once

#include <gmock/gmock.h>

extern "C" {
struct bio_job {
    time_t time; /* Time at which the job was created. */
    /* Job specific arguments.*/
    int fd;            /* Fd for file based background jobs */
    void *free_fn;     /* Function that will free the provided arguments */
    void *free_args[]; /* List of arguments to be passed to the free function */
};

void bioSubmitJob(int type, struct bio_job *job);
void *bioProcessBackgroundJobs(void *arg);
}
