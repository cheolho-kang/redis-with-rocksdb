#pragma once

#include <gmock/gmock.h>

#ifdef __cplusplus
extern "C" {
#endif

struct bio_job {
    time_t time; /* Time at which the job was created. */
    /* Job specific arguments.*/
    int fd; /* Fd for file based background jobs */
    void *free_fn; /* Function that will free the provided arguments */
    void *free_args[]; /* List of arguments to be passed to the free function */
};

void bioInit(void);
void bioCreateForFlush(int type, int arg_count, ...);

// void spyBioSubmitJob(int type, struct bio_job *job);
// void spyBioCreateForFlush(int type, int arg_count, void* arg1, void* arg2, void* arg3);
#ifdef __cplusplus
}
#endif

class BioMock {
public:
    BioMock(void) {};
    virtual ~BioMock(void) {};

    virtual void BioSubmitJob(int type, struct bio_job *job) = 0;
    virtual void BioCreateForFlush(int type, int argCount, void* arg1, void* arg2, void* arg3) = 0;
};
