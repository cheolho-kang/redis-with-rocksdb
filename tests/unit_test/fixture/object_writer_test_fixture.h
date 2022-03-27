#pragma once

#include <string>
#include <unordered_map>

#include "src/sds.h"
#include "src/server.h"
#include "fake/server_spy.h"

#define MAX_LENGTH 1024 * 1024

class ObjectWriterTestFixture {
  public:
    struct dictType hashDictType = {
        dictSdsHash,       /* hash function */
        NULL,              /* key dup */
        NULL,              /* val dup */
        dictSdsKeyCompare, /* key compare */
        dictSdsDestructor, /* key destructor */
        dictSdsDestructor  /* val destructor */
    };
    dictType dbExpiresDictType = {
        dictSdsHash,       /* hash function */
        NULL,              /* key dup */
        NULL,              /* val dup */
        dictSdsKeyCompare, /* key compare */
        NULL,              /* key destructor */
        NULL,              /* val destructor */
        dictExpandAllowed  /* allow to expand */
    };

    ObjectWriterTestFixture(void) = delete;
    ObjectWriterTestFixture(struct redisServer *redisServer);
    virtual ~ObjectWriterTestFixture(void) {}

    robj *CreateStringObject(char *target);
    robj *CreateHashObject(void);
    dictEntry *CreateDictEntry(char *key, robj *targetObject);

    void AddStringToDict(dict *targetDict, char *field, char *value);
    void PutObjects(robj *keyObject, robj *targetObject, sds filed, sds value);
    void PutObjectsWithMBSize(robj *keyObject, robj *targetObject, uint64_t mbSizeToFill);

  private:
    void _AddToWriteList(char *key, char *filed, char *value);
    std::string _GenerateRandomString(uint64_t length);

    struct redisServer *redisServer;
    std::unordered_map<char *, std::unordered_map<char *, char *>> addedKeyValueLists;
};
