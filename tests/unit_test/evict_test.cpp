#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "fake/bio_mock.h"
#include "fixture/object_writer_test_fixture.h"
#include "fixture/redis_server_test_fixture.h"
#include "src/bio.h"
#include "src/server.h"

using ::testing::NiceMock;

class EvictTestFixture : public RedisServerTestFixture, public ::testing::Test {
  public:
    EvictTestFixture(void) {}
    virtual ~EvictTestFixture(void) {}
    virtual void SetUp(void) {}
    virtual void TearDown(void) { delete objectWriter; }

  protected:
};

TEST_F(EvictTestFixture, performEvictionsToRocksdbIfExecutedWhenMemUsed90Percent) {
    // Given
    extern MockBio *mockBio;
    mockBio = new NiceMock<MockBio>;

    char const *expectKey1 = "key1";
    char const *expectField1 = "field1";
    char const *expectField2 = "field2";
    char const *expectValue1 = "Hello";
    char const *expectValue2 = "World";

    robj *keyObj1 = objectWriter->CreateStringObject((char *)expectKey1);
    robj *valObj1 = objectWriter->CreateHashObject();

    objectWriter->PutObjects(keyObj1, valObj1, (char *)expectField1, (char *)expectValue1);
    objectWriter->PutObjects(keyObj1, valObj1, (char *)expectField2, (char *)expectValue2);

    char const *expectKey2 = "key2";
    robj *keyObj2 = objectWriter->CreateStringObject((char *)expectKey2);
    robj *valObj2 = objectWriter->CreateHashObject();

    // When
    objectWriter->PutObjectsWithMBSize(keyObj2, valObj2, 80);

    ON_CALL(*mockBio, BioCreateForFlush).WillByDefault([&](int type, int argCount, void *arg1, void *arg2, void *arg3) {
        bioCreateForFlush(type, argCount, arg1, arg2, arg3);
    });
    performEvictionsToRocksdb();

    // Then
}
