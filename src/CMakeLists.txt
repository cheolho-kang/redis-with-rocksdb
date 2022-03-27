cmake_minimum_required(VERSION 3.8)

# Add target sources
set(server_src
    t_rocksdb.c
    rocksdb_util_c++.cc
    queue_util.cc
    adlist.c
    quicklist.c
    ae.c
    anet.c
    dict.c
    server.c
    sds.c
    zmalloc.c
    lzf_c.c
    lzf_d.c
    pqsort.c
    zipmap.c
    sha1.c
    ziplist.c
    release.c
    networking.c
    util.c
    object.c
    db.c
    replication.c
    rdb.c
    t_string.c
    t_list.c
    t_set.c
    t_zset.c
    t_hash.c
    config.c
    aof.c
    pubsub.c
    multi.c
    debug.c
    sort.c
    intset.c
    syncio.c
    cluster.c
    crc16.c
    endianconv.c
    slowlog.c
    scripting.c
    bio.c
    rio.c
    rand.c
    memtest.c
    crcspeed.c
    crc64.c
    bitops.c
    sentinel.c
    notify.c
    setproctitle.c
    blocked.c
    hyperloglog.c
    latency.c
    sparkline.c
    redis-check-rdb.c
    redis-check-aof.c
    geo.c
    lazyfree.c
    module.c
    evict.c
    expire.c
    geohash.c
    geohash_helper.c
    childinfo.c
    defrag.c
    siphash.c
    rax.c
    t_stream.c
    listpack.c
    localtime.c
    lolwut.c
    lolwut5.c
    lolwut6.c
    acl.c
    gopher.c
    tracking.c
    connection.c
    tls.c
    sha256.c
    timeout.c
    setcpuaffinity.c
    monotonic.c
    mt19937-64.c
)

set(client_src
    anet.c
    adlist.c
    dict.c
    redis-cli.c
    zmalloc.c
    release.c
    ae.c
    crcspeed.c
    crc64.c
    siphash.c
    crc16.c
    monotonic.c
    cli_common.c
    mt19937-64.c
)

# Set target
add_executable(${PROJECT_BINARY} ${server_src})
add_dependencies(${PROJECT_BINARY} jemalloc lua hiredis rocksdb snappy)
set_target_properties(${PROJECT_BINARY} PROPERTIES LINKER_LANGUAGE CXX)
target_compile_options(${PROJECT_BINARY} PRIVATE -Wall -Wpedantic -g -Wsign-compare -Wreturn-type -fno-omit-frame-pointer -O0)
target_include_directories(${PROJECT_BINARY} ${EXTERNAL_INCS})
target_link_libraries(${PROJECT_BINARY} ${EXTERNAL_LIBS})
target_compile_definitions(${PROJECT_BINARY} PRIVATE -DROCKSDB)

# Set client
add_executable(redis-cli ${client_src})
add_dependencies(redis-cli jemalloc hiredis hdr_histogram linenoise rocksdb)
target_compile_options(redis-cli  PRIVATE -Wall -Wpedantic -g -Wsign-compare -Wreturn-type -fno-omit-frame-pointer -O0 -pthread)
target_include_directories(redis-cli  ${EXTERNAL_INCS})
target_link_libraries(redis-cli  PRIVATE ${EXTERNAL_LIBS})

# Set library
add_library(${PROJECT_BINARY}lib
    STATIC ${server_src}
)
add_dependencies(${PROJECT_BINARY}lib jemalloc lua hiredis rocksdb)
target_compile_features(${PROJECT_BINARY}lib PRIVATE cxx_std_11)
target_compile_options(${PROJECT_BINARY}lib PRIVATE -Wall -Wpedantic -g -Wsign-compare -Wreturn-type -fno-omit-frame-pointer -O0 -pthread)
target_include_directories(${PROJECT_BINARY}lib ${EXTERNAL_INCS})
target_compile_definitions(${PROJECT_BINARY}lib PRIVATE -DUNIT_TEST -DROCKSDB)
