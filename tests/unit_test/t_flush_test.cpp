#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <iostream>
#include <stdarg.h>

#include "dict.h"
#include "fake/bio_mock.h"
#include "fake/bio_spy.h"
#include "src/bio.h"
#include "src/server.h"

TEST(TFlushTest, flushDictToFlashIfExecutedSuccesfully) {
    // Given
    extern MockBio *mockBio;
    mockBio = new NiceMock<MockBio>;

    char const *expectArg2 = "Hello";
    char const *expectArg3 = "World";

    struct dictEntry de;
    struct redisObject robj;
    robj.where = LOC_REDIS;
    robj.ptr = (void *)expectArg3;

    de.key = (void *)expectArg2;
    de.val = (void *)&robj;

    ON_CALL(*mockBio, BioCreateForFlush).WillByDefault([&](int type, int argCount, void *arg1, void *arg2, void *arg3) {
        int expectArgCount = 3;
        EXPECT_EQ(expectArgCount, argCount);

        void *expectArg1 = nullptr;
        EXPECT_EQ(expectArg1, arg1);
        EXPECT_EQ(expectArg2, (char *)arg2);
        EXPECT_EQ(expectArg3, (char *)(((struct redisObject *)arg3)->ptr));

        int expectLoc = LOC_FLUSHING;
        EXPECT_EQ(expectLoc, (int)(((struct redisObject *)arg3)->where));
    });

    // When
    flushDictToFlash(nullptr, &de);

    // Then

    delete mockBio;
}

TEST(TFlushTest, flushDictToFlashWithBioCreateForFlushIfExecutedSuccesfully) {
    // Given
    extern MockBio *mockBio;
    mockBio = new NiceMock<MockBio>;

    char const *expectArg1 = "Hello";
    char const *expectArg2 = "World";

    struct dictEntry de;
    struct redisObject robj;
    robj.where = LOC_REDIS;
    robj.ptr = (void *)expectArg2;

    de.key = (void *)expectArg1;
    de.val = (void *)&robj;

    ON_CALL(*mockBio, BioCreateForFlush).WillByDefault([&](int type, int argCount, void *arg1, void *arg2, void *arg3) {
        bioCreateForFlush(type, argCount, arg1, arg2, arg3);
        });

    ON_CALL(*mockBio, BioSubmitJob).WillByDefault([&](int type, struct bio_job *job) {
        int expectType = 3;
        EXPECT_EQ(expectType, type);
        // EXPECT_EQ(expectArg1, *(char *)(job->free_args[0]));
        // EXPECT_EQ(expectArg2, *(int *)(job->free_args[1]));
        // EXPECT_EQ(expectArg3, (char *)(job->free_args[2]));
    });

    // When
    flushDictToFlash(nullptr, &de);

    // Then

    delete mockBio;
}
