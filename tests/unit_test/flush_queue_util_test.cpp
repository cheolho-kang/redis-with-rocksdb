#include "src/queue_util.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string.h>

#include "src/dict.h"
#include "src/server.h"
#include "fixture/object_writer_test_fixture.h"

TEST(FlushQueue, EnqueueIfExecutedSuccesfully) {
    // Given
    FlushQueue queue;
    ObjectWriterTestFixture* objectWriter = new ObjectWriterTestFixture(nullptr);

    char const *expectKey1 = "key1";
    char const *expectField1 = "field1";
    char const *expectField2 = "field2";
    char const *expectValue1 = "Hello";
    char const *expectValue2 = "World";

    robj *valueObject = objectWriter->CreateHashObject();
    objectWriter->AddStringToDict((dict*)valueObject->ptr, (char*)expectField1, (char*)expectValue1);
    objectWriter->AddStringToDict((dict*)valueObject->ptr, (char*)expectField2, (char*)expectValue2);

    dictEntry* de = objectWriter->CreateDictEntry((char*)expectKey1, valueObject);

    // When
    queue.Enqueue(de);

    // Then
    int expectSize = 1;
    EXPECT_EQ(expectSize, queue.IsSize());

    dictEntry* actual = queue.Dequeue();
    EXPECT_TRUE(strcmp(expectKey1, (const char*)actual->key) == 0);

    EXPECT_EQ(valueObject, (robj*)actual->v.val);
    delete objectWriter;
}
