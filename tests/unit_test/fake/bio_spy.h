#pragma once

#include <gmock/gmock.h>

#ifdef __cplusplus
extern "C" {
#endif

struct bio_job {
    time_t time; /* Time at which the job was created. */
    /* Job specific arguments.*/
    int fd;            /* Fd for file based background jobs */
    void *free_fn;     /* Function that will free the provided arguments */
    void *free_args[]; /* List of arguments to be passed to the free function */
};

void bioInit(void);
void bioCreateForFlush(int type, int arg_count, ...);
void bioSubmitJob(int type, struct bio_job *job);
void *bioProcessBackgroundJobs(void *arg);
unsigned long long bioWaitStepOfType(int type);
void bioKillThreads(void);

#ifdef __cplusplus
}
#endif
