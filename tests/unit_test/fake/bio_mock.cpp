#include "bio_mock.h"

MockBio *mockBio;

void spyBioSubmitJob(int type, struct bio_job *job) { return mockBio->BioSubmitJob(type, job); }
void spyBioCreateForFlush(int type, int arg_count, void *arg1, void *arg2, void *arg3) {
    return mockBio->BioCreateForFlush(type, arg_count, arg1, arg2, arg3);
}
