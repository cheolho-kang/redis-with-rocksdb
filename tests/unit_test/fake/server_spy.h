#pragma once

extern "C" {
void initServerConfig(void);
struct rocksdb_options_t *initRocksdbOptions(void);
struct rocksdb_t *initRocksDB(rocksdb_options_t *options, char *db_path, int dbnum);
void initServer(void);
int dictExpandAllowed(size_t moreMem, double usedRatio);
}
