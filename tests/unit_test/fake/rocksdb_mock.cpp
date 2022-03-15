#include "rocksdb_mock.h"

#if defined(__cplusplus)
extern "C" {
#endif

MockRocksdb *mockRocksdb;

void spyRocksdb_put(
    rocksdb_t* db,
    const rocksdb_writeoptions_t* options,
    const char* key, size_t keylen,
    const char* val, size_t vallen,
    char** errptr) {
        return mockRocksdb->rocksdb_put(db, options, key, keylen, val, vallen, errptr);
    }

#ifdef __cplusplus
}
#endif
