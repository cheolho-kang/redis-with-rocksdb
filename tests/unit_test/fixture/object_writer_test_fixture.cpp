#include "object_writer_test_fixture.h"

#include "dict.h"
#include "src/server.h"

extern "C"{
#include "src/zmalloc.h"
}

ObjectWriterTestFixture::ObjectWriterTestFixture(struct redisServer *redisServer)
: redisServer(redisServer) {
}

robj *ObjectWriterTestFixture::CreateStringObject(char *target) {
    robj *result = createStringObject(target, strlen(target));

    return result;
}

robj *ObjectWriterTestFixture::CreateHashObject(void) {
    robj *result = createHashObject();
    hashTypeConvert(result, OBJ_ENCODING_HT);
    dict *dict = dictCreate(&hashDictType, NULL);
    result->ptr = dict;

    return result;
}

dictEntry *ObjectWriterTestFixture::CreateDictEntry(char *key, robj *targetObject) {
    dictEntry *entry;
    entry = (dictEntry *)zmalloc(sizeof(*entry));

    sds keyToAdd = sdsnew(key);
    entry->key = keyToAdd;
    entry->v.val = targetObject;

    return entry;
}

void ObjectWriterTestFixture::AddStringToDict(dict *targetDict, char *field, char *value){
    sds fieldToWrite = sdsnew(field);
    sds valueToWrite = sdsnew(value);

    dictAdd(targetDict, (void *)fieldToWrite, (void *)valueToWrite);
}

void ObjectWriterTestFixture::PutObjects(robj *keyObject, robj *targetObject, char *field, char *value) {
    AddStringToDict((dict *)targetObject->ptr, field, value);

    robj *foundObject = lookupKeyWrite(redisServer->db, keyObject);
    if (foundObject == NULL) {
        dbAdd(redisServer->db, keyObject, targetObject);
    }
    _AddToWriteList((char*)keyObject->ptr, field, value);
}

void ObjectWriterTestFixture::PutObjectsWithMBSize(robj *keyObject, robj *targetObject, uint64_t mbSizeToFill) {
    uint64_t remainSize = mbSizeToFill * 1024 * 1024;

    while (remainSize != 0) {
        uint64_t valueLength = remainSize > MAX_LENGTH ? MAX_LENGTH : remainSize / 2;
        remainSize -= valueLength;
        std::string value = _GenerateRandomString(valueLength);

        uint64_t filedLength = remainSize > MAX_LENGTH ? MAX_LENGTH : remainSize;
        remainSize -= filedLength;
        std::string field = _GenerateRandomString(filedLength);

        PutObjects(keyObject, targetObject, (char*)field.c_str(), (char*)value.c_str());
    }
}

void ObjectWriterTestFixture::_AddToWriteList(char *key, char *field, char *value) {
    std::unordered_map<char *, char *> targetValue;

    if (addedKeyValueLists.find(key) == addedKeyValueLists.end()) {
        targetValue[field] = value;
        addedKeyValueLists.emplace(key, targetValue);
    } else {
        addedKeyValueLists[key][field] = value;
    }
}

std::string ObjectWriterTestFixture::_GenerateRandomString(uint64_t length) {
    auto randChar = []() -> char {
        const char charSet[] = "0123456789"
                               "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                               "abcdefghijklmnopqrstuvwxyz";
        const size_t maxIndex = (sizeof(charSet) - 1);
        return charSet[rand() % maxIndex];
    };
    std::string str(length, 0);
    std::generate_n(str.begin(), length, randChar);
    str += '\0';
    return str;
}
