#pragma once

#ifdef __cplusplus
extern "C" {
#endif

struct redisDb;
struct dictEntry;
int flushDictToFlash(redisDb *db, dictEntry *hashDataEntry);

#ifdef __cplusplus
}
#endif
