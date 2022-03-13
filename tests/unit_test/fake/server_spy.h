#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define LL_WARNING 3
#define LRU_BITS 24

#define LOC_REDIS 0
#define LOC_FLUSHING 1
#define LOC_ROCKSDB 2

struct rocksdb_options_t;
struct rocksdb_t;

struct redisServer {
    char *rocksdb_path;
    int rocksdb_loglevel;
    int rocksdb_write_buffer_size_mb; /* Memory size of a single rocksdb write buffer */
    int rocksdb_block_cache_size_mb;  /* memory size of a rocksdb block cache */
    int rocksdb_options_set_max_bytes_for_level_multiplier;
};

struct redisDb {
    struct rocksdb_t *rocksdb;
    struct rocksdb_options_t *rocksdb_options; /* options of rocksdb */
};

struct redisObject {
    volatile unsigned where : 2;
    unsigned type : 4;
    unsigned encoding : 4;
    unsigned lru : LRU_BITS;
    int refcount;
    void *ptr;
};

void initServerConfig(void);
struct rocksdb_options_t *initRocksdbOptions(void);
struct rocksdb_t *initRocksDB(rocksdb_options_t *options, char *db_path, int dbnum);

#ifdef __cplusplus
}
#endif