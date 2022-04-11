#pragma once

#include "rocksdb/c.h"
#include "rocksdb/db.h"
#include "rocksdb/options.h"

struct rocksdb_options_t         { rocksdb::Options           rep; };
struct rocksdb_t                 { rocksdb::DB*               rep; };
