#ifndef ROCKSDB_TIER_H_
#define ROCKSDB_TIER_H_

#include "server.h"
#include "dict.h"

#ifdef __cplusplus
extern "C" {
#endif

int flushDictToFlash(redisDb *db, dictEntry *hashDataEntry);

#ifdef __cplusplus
}
#endif

#endif