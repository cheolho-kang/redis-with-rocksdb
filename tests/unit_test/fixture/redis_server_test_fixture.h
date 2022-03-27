#pragma once

struct redisServer;
class ObjectWriterTestFixture;
class RedisServerTestFixture {
  public:
    RedisServerTestFixture(void);
    virtual ~RedisServerTestFixture(void);

private:
    void _InitServer(void);

protected:
    struct redisServer *redisServer;
    ObjectWriterTestFixture *objectWriter;
};
