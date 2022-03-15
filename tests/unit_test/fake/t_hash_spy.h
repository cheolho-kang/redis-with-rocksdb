#pragma once

#include "sds_spy.h"
#include "server_spy.h"

#ifdef __cplusplus
extern "C" {
#endif

void hashTypeConvert(robj *o, int enc);
hashTypeIterator *hashTypeInitIterator(robj *subject);
int hashTypeNext(hashTypeIterator *hi);
sds hashTypeCurrentFromHashTable(hashTypeIterator *hi, int what);

#ifdef __cplusplus
}
#endif
