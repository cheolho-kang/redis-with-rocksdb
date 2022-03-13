#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>

#include "fake/rocksdb_spy.h"
#include "fake/server_spy.h"
#include "rocksdb/advanced_options.h"
#include "rocksdb/env.h"

using namespace testing;

extern struct redisServer server;

class ServerTestFixture : public ::testing::Test {
  public:
    ServerTestFixture(void) {}
    virtual ~ServerTestFixture(void) {}
    virtual void SetUp(void) {}
    virtual void TearDown(void) {}
};

TEST_F(ServerTestFixture, initServerIfExecutedSuccesfully) {
    // When
    initServerConfig();

    // Then
    EXPECT_EQ(std::string(server.rocksdb_path), ROCKSDB_PATH);
    EXPECT_EQ(server.rocksdb_loglevel, LL_WARNING);
    EXPECT_EQ(server.rocksdb_write_buffer_size_mb, ROCKSDB_WRITE_BUFFER_SIZE_MB);
    EXPECT_EQ(server.rocksdb_block_cache_size_mb, ROCKSDB_BLOCK_CACHE_SIZE_MB);
    EXPECT_EQ(server.rocksdb_options_set_max_bytes_for_level_multiplier, ROCKSDB_LEVEL_MULTIPLIER);

    // Given
    struct rocksdb_options_t *resultOptions;

    // When
    resultOptions = initRocksdbOptions();

    // Then
    EXPECT_EQ(resultOptions->rep.create_if_missing, true);
    EXPECT_EQ(resultOptions->rep.info_log_level, rocksdb::InfoLogLevel::WARN_LEVEL);
    EXPECT_EQ(resultOptions->rep.max_background_jobs, 6);
    EXPECT_EQ(resultOptions->rep.max_background_compactions, 3);
    EXPECT_EQ(resultOptions->rep.max_background_flushes, 3);
    EXPECT_EQ(resultOptions->rep.compaction_style, rocksdb::CompactionStyle::kCompactionStyleLevel);
    EXPECT_EQ(resultOptions->rep.write_buffer_size, server.rocksdb_write_buffer_size_mb * 1024 * 1024);

    // When
    struct rocksdb_t *resultObject = initRocksDB(resultOptions, server.rocksdb_path, 0);

    // Then
    EXPECT_TRUE(resultObject != nullptr);
}
