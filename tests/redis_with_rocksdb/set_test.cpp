#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace testing;

class SetTestFixture : public ::testing::Test {
public:
    SetTestFixture(void) {}
    virtual ~SetTestFixture(void) {}
    virtual void SetUp(void) {}
    virtual void TearDown(void) {}
};

TEST_F(SetTestFixture, addDataIfExecutedSuccesfully) {
    // Given
    bool result;

    // When
    result = true;

    // Then
    bool expected = true;
    EXPECT_EQ(result, expected);
}
