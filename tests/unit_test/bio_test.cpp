#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <iostream>
#include <string.h>

#include "fake/bio_mock.h"
#include "fake/bio_spy.h"
#include "fake/rocksdb_mock.h"
#include "fake/server_spy.h"
#include "fake/t_hash_spy.h"
#include "src/rocksdb_util_c++.h"

extern "C" {
#include "src/dict.h"
}

using ::testing::NiceMock;

#define BIO_FLUSH_TO_ROCKSDB 3
#define OBJ_ENCODING_HT 2 /* Encoded as hash table */

extern MockRocksdb *mockRocksdb;
class BioTestFixture : public ::testing::Test {
  public:
    BioTestFixture(void) {}
    virtual ~BioTestFixture(void) {}
    virtual void SetUp(void) {
        mockRocksdb = new NiceMock<MockRocksdb>;
        bioInit();
    }
    virtual void TearDown(void) {
        delete mockRocksdb;
        // bioKillThreads();
    }

    struct dictType hashDictType = {
        dictSdsHash,       /* hash function */
        NULL,              /* key dup */
        NULL,              /* val dup */
        dictSdsKeyCompare, /* key compare */
        dictSdsDestructor, /* key destructor */
        dictSdsDestructor  /* val destructor */
    };

  protected:
    struct redisDb redisDb;
};

TEST(BioTest, bioCreateWithArgumentsIfExecutedSuccesfully) {
    // Given
    extern MockBio *mockBio;
    mockBio = new NiceMock<MockBio>;

    int expectType = BIO_FLUSH_TO_ROCKSDB;
    char expectArg1 = 'K';
    int expectArg2 = 9;
    char const *expectArg3 = "Hello World";

    int type = expectType;
    int argc = 3;
    char arg1 = expectArg1;
    int arg2 = expectArg2;
    char const *arg3 = expectArg3;

    ON_CALL(*mockBio, BioSubmitJob).WillByDefault([&](int type, struct bio_job *job) {
        EXPECT_EQ(expectType, type);
        EXPECT_EQ(expectArg1, *(char *)(job->free_args[0]));
        EXPECT_EQ(expectArg2, *(int *)(job->free_args[1]));
        EXPECT_EQ(expectArg3, (char *)(job->free_args[2]));
    });
    // When
    bioCreateForFlush(BIO_FLUSH_TO_ROCKSDB, argc, (void *)&arg1, (void *)&arg2, (void *)arg3);

    // Then
    delete mockBio;
}

TEST_F(BioTestFixture, bioProcessBackgroundJobsIfExecutedToFlush) {
    // Given
    extern MockBio *mockBio;
    mockBio = new NiceMock<MockBio>;

    robj *val_obj = createHashObject();
    hashTypeConvert(val_obj, OBJ_ENCODING_HT);
    dict *dict = dictCreate(&hashDictType, NULL);
    val_obj->ptr = dict;

    sds expectKey = sdsnew("MyKey");
    sds expectField1 = sdsnew("field1");
    sds expectValue1 = sdsnew("Hello");
    sds expectField2 = sdsnew("field2");
    sds expectValue2 = sdsnew("World");

    dictAdd(dict, (void *)expectField1, (void *)expectValue1);
    dictAdd(dict, (void *)expectField2, (void *)expectValue2);

    // When
    ON_CALL(*mockBio, BioSubmitJob).WillByDefault([&](int type, struct bio_job *job) { bioSubmitJob(type, job); });
    ON_CALL(*mockRocksdb, rocksdb_put)
        .WillByDefault([&](rocksdb_t *db, const rocksdb_writeoptions_t *options, const char *key, size_t keylen,
                           const char *val, size_t vallen, char **errptr) {
            EXPECT_EQ(expectKey, key);
            EXPECT_EQ(strlen(expectKey), keylen);

            objectList *expectEncoded = createObjectList((void *)val_obj);
            EXPECT_TRUE(strcmp(expectEncoded->ptr, val) == 0);
            EXPECT_EQ(expectEncoded->allocated_length, vallen);
        });
    bioCreateForFlush(BIO_FLUSH_TO_ROCKSDB, 3, (void *)&redisDb, (void *)expectKey, (void *)val_obj);

    // Then
    bioWaitStepOfType(BIO_FLUSH_TO_ROCKSDB);

    dictRelease(dict);
    delete mockBio;
}
