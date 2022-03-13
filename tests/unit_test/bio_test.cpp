#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <iostream>

#include "fake/bio_mock.h"
#include "fake/bio_spy.h"

using ::testing::NiceMock;

#define BIO_UINIT_TEST 7

TEST(BioTest, bioCreateWithArgumentsIfExecutedSuccesfully) {
    // Given
    
    extern MockBio *bio;
    bio = new NiceMock<MockBio>;

    int expectType = BIO_UINIT_TEST;
    char expectArg1 = 'K';
    int expectArg2 = 9;
    char const *expectArg3 = "Hello World";

    int type = expectType;
    int argc = 3;
    char arg1 = expectArg1;
    int arg2 = expectArg2;
    char const *arg3 = expectArg3;

    ON_CALL(*bio, BioSubmitJob).WillByDefault([&](int type, struct bio_job *job) {
        EXPECT_EQ(expectType, type);
        EXPECT_EQ(expectArg1, *(char *)(job->free_args[0]));
        EXPECT_EQ(expectArg2, *(int *)(job->free_args[1]));
        EXPECT_EQ(expectArg3, (char *)(job->free_args[2]));
    });
    // When
    bioCreateForFlush(BIO_UINIT_TEST, argc, (void *)&arg1, (void *)&arg2, (void *)arg3);

    // Then
    delete bio;
}
