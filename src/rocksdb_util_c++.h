#ifndef ROCKSDB_UTIL_C___H_
#define ROCKSDB_UTIL_C___H_

#ifdef __cplusplus
extern "C" {
#endif

// server.h
struct redisDb;

static const unsigned char VALID_MARK = 0xAD;
static const unsigned char STRING_DELIMITER = 0xCE;

struct dict;
struct dictEntry;

typedef struct objectList {
    int length;
    int allocated_length;
    char *ptr;
} objectList;

typedef struct decodedObjectList {
    int length;
    int allocated_length;
    char **fields;
    char **values;
} decodedObjectList;

objectList *createObjectList(void *object);
decodedObjectList *decodeRocksdbValues(const char *values, unsigned long long maxOffset);

void *queueCreate(void);
void queueDestroy(redisDb *db);
void queueEnqueue(redisDb *db, dictEntry* entry);
dictEntry *queueDequeue(redisDb *db);
int queueSize(redisDb *db);

#ifdef __cplusplus
}
#endif

#endif /* ROCKSDB_UTIL_C___H_ */
