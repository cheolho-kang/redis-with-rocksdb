#pragma once

#include <gmock/gmock.h>

using ::testing::NiceMock;

typedef struct rocksdb_t rocksdb_t;
typedef struct rocksdb_writeoptions_t rocksdb_writeoptions_t;

class RocksdbMock {
  public:
    RocksdbMock(void){};
    virtual ~RocksdbMock(void){};

    virtual void rocksdb_put(
        rocksdb_t* db,
        const rocksdb_writeoptions_t* options,
        const char* key, size_t keylen,
        const char* val, size_t vallen,
        char** errptr) = 0;
};

class MockRocksdb : public RocksdbMock {
public:
    MOCK_METHOD(void, rocksdb_put, (rocksdb_t* db,
        const rocksdb_writeoptions_t* options,
        const char* key, size_t keylen,
        const char* val, size_t vallen,
        char** errptr), (override));
};

#ifdef __cplusplus
extern "C" {
#endif

void spyRocksdb_put(
    rocksdb_t* db,
    const rocksdb_writeoptions_t* options,
    const char* key, size_t keylen,
    const char* val, size_t vallen,
    char** errptr);

#ifdef __cplusplus
}
#endif
