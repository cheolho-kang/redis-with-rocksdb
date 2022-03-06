#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define LL_WARNING 3
struct rocksdb_options_t;
struct rocksdb_t;

struct redisServer {
    char *rocksdb_path;
    int rocksdb_loglevel;
    int rocksdb_write_buffer_size_mb; /* Memory size of a single rocksdb write buffer */
    int rocksdb_block_cache_size_mb;  /* memory size of a rocksdb block cache */
    int rocksdb_options_set_max_bytes_for_level_multiplier;
};

void initServerConfig(void);
struct rocksdb_options_t *initRocksdbOptions(void);
struct rocksdb_t *initRocksDB(rocksdb_options_t *options, char *db_path, int dbnum);

#ifdef __cplusplus
}
#endif