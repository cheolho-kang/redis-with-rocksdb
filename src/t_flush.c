#ifdef ROCKSDB
#include "server.h"
#include "bio.h"

#ifdef UNIT_TEST
void spyBioCreateForFlush(int type, int arg_count, void* arg1, void* arg2, void* arg3);
#endif

int flushDictToFlash(redisDb *db, dictEntry *hashDataEntry){
    if(hashDataEntry == NULL)
        return C_ERR;

    sds flashKey = dictGetKey(hashDataEntry);
    robj *hashDataObj = dictGetVal(hashDataEntry);
    hashDataObj->where = LOC_FLUSHING;

#ifdef UNIT_TEST
    spyBioCreateForFlush(BIO_FLUSH_TO_ROCKSDB, 3, (void*)db, (void*)flashKey, (void*)hashDataObj);
#else
    bioCreateForFlush(BIO_FLUSH_TO_ROCKSDB, 3, db, flashKey, hashDataObj);
#endif
    return C_OK;
}

#endif
