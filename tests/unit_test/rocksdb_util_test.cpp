#include "src/rocksdb_util_c++.h"

#include <fstream>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <iostream>
#include <string.h>

#include "fake/sds_spy.h"
#include "fake/server_spy.h"
#include "fake/t_hash_spy.h"

extern "C" {
#include "src/dict.h"
#include "src/zmalloc.h"
}

using ::testing::NiceMock;

#define BIO_FLUSH_TO_ROCKSDB 3
#define OBJ_ENCODING_HT 2 /* Encoded as hash table */

class RocksdbUtilTestFixture : public ::testing::Test {
  public:
    RocksdbUtilTestFixture(void) {}
    virtual ~RocksdbUtilTestFixture(void) {}
    virtual void SetUp(void) {}
    virtual void TearDown(void) {}

    struct dictType hashDictType = {
        dictSdsHash,       /* hash function */
        NULL,              /* key dup */
        NULL,              /* val dup */
        dictSdsKeyCompare, /* key compare */
        dictSdsDestructor, /* key destructor */
        dictSdsDestructor  /* val destructor */
    };
};

TEST_F(RocksdbUtilTestFixture, createValuesObjectIfExecutedSuccesfully) {
    // Given
    std::ofstream writeFile;
    const char *fileName = "unittest_rocksdbUtilTest_1.txt";
    writeFile.open(fileName); //파일 열기(파일이 없으면 만들어짐)

    robj *val_obj = createHashObject();
    hashTypeConvert(val_obj, OBJ_ENCODING_HT);
    dict *dict = dictCreate(&hashDictType, NULL);
    val_obj->ptr = dict;

    sds field1 = sdsnew("field1");
    sds value1 = sdsnew("Hello");
    sds field2 = sdsnew("field2");
    sds value2 = sdsnew("World");

    dictAdd(dict, (void *)field1, (void *)value1);
    dictAdd(dict, (void *)field2, (void *)value2);

    // When
    objectList *actualEncoded = createObjectList((void *)val_obj);
    writeFile.write(actualEncoded->ptr, actualEncoded->allocated_length);
    writeFile.close();

    // Then
    std::ifstream readFile(fileName);
    std::string s;

    getline(readFile, s);
    decodedObjectList *actualDecoded = decodeRocksdbValues(s.c_str(), s.length());
    readFile.close();

    EXPECT_EQ(2, actualDecoded->length);

    // Ordering can be wrong cause of hash ordering
    EXPECT_TRUE(strcmp(field1, actualDecoded->fields[0]) == 0 || strcmp(field2, actualDecoded->fields[0]) == 0);
    EXPECT_TRUE(strcmp(value1, actualDecoded->values[0]) == 0 || strcmp(value2, actualDecoded->values[0]) == 0);
    EXPECT_TRUE(strcmp(field2, actualDecoded->fields[1]) == 0 || strcmp(field1, actualDecoded->fields[1]) == 0);
    EXPECT_TRUE(strcmp(value2, actualDecoded->values[1]) == 0 || strcmp(value1, actualDecoded->values[1]) == 0);

    dictRelease(dict);

    for (int i = 0; i < actualDecoded->length; i++) {
        sdsfree(actualDecoded->fields[i]);
        sdsfree(actualDecoded->values[i]);
    }
    zfree(actualEncoded);
    zfree(actualDecoded);
    dictRelease(dict);
}
