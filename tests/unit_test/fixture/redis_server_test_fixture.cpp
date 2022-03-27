#include "redis_server_test_fixture.h"

#include <memory.h>

#include "fake/rocksdb_spy.h"
#include "object_writer_test_fixture.h"
#include "rocksdb/c.h"
#include "src/adlist.h"
#include "src/rocksdb_util_c++.h"
#include "src/server.h"
#include "src/zmalloc.h"

RedisServerTestFixture::RedisServerTestFixture(void) {
    redisServer = &server;
    objectWriter = new ObjectWriterTestFixture(redisServer);

    _InitServer();
}

RedisServerTestFixture::~RedisServerTestFixture(void) {
    delete objectWriter;

    // TODO (cheolho.kang): close redis db
    // TODO (cheolho.kang): Release hash dicts in db
    dictRelease(redisServer->db->expires);

    rocksdb_close(redisServer->db->rocksdb);
    free(redisServer->db);
}

void RedisServerTestFixture::_InitServer(void) {
    initServerConfig();
    redisServer->flush_trigger_ratio_of_maxmemory = 80;
    redisServer->maxmemory_policy = MAXMEMORY_ALLKEYS_LRU;
    redisServer->dbnum = 1;
    redisServer->maxmemory = 10 * 1024 * 1024;
    redisServer->slaves = listCreate();

    struct rocksdb_options_t *rocksdbOptions;
    struct rocksdb_t *rocksdbObject;
    rocksdbOptions = initRocksdbOptions();
    rocksdbObject = initRocksDB(rocksdbOptions, redisServer->rocksdb_path, 0);

    redisDb *db = (redisDb *)zmalloc(sizeof(redisDb) * redisServer->dbnum);
    db->rocksdb = rocksdbObject;
    db->rocksdb_options = rocksdbOptions;
    db->queueForFlush = queueCreate();
    db->_dict = dictCreate(&objectWriter->hashDictType, nullptr);
    db->expires = dictCreate(&objectWriter->dbExpiresDictType, NULL);

    evictionPoolAlloc();
    redisServer->db = db;
}