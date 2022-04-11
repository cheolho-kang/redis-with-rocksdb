#pragma once

#include <gmock/gmock.h>

using ::testing::NiceMock;

class BioMock {
  public:
    BioMock(void){};
    virtual ~BioMock(void){};

    virtual void BioSubmitJob(int type, struct bio_job *job) = 0;
    virtual void BioCreateForFlush(int type, int argCount, void *arg1, void *arg2, void *arg3) = 0;
};
class MockBio : public BioMock {
  public:
    MOCK_METHOD(void, BioSubmitJob, (int type, struct bio_job *job), (override));
    MOCK_METHOD(void, BioCreateForFlush, (int type, int argCount, void *arg1, void *arg2, void *arg3), (override));
};

extern "C" {
void spyBioSubmitJob(int type, struct bio_job *job);
void spyBioCreateForFlush(int type, int arg_count, void *arg1, void *arg2, void *arg3);
}