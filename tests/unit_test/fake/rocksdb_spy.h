#pragma once

#include "rocksdb/c.h"
#include "rocksdb/db.h"
#include "rocksdb/options.h"

#define ROCKSDB_PATH "/tmp/redis_rocksdb" /* rocksdb database path */
#define ROCKSDB_BLOCK_CACHE_SIZE_MB 64
#define ROCKSDB_WRITE_BUFFER_SIZE_MB 64
#define ROCKSDB_LEVEL_MULTIPLIER 10

struct rocksdb_options_t         { rocksdb::Options           rep; };
struct rocksdb_t                 { rocksdb::DB*               rep; };
