#include "rocksdb_util_c++.h"

#include <string.h>
#include <string>
#include <vector>

#include "dict.h"
#include "rocksdb_tier.h"
#include "server.h"
#include "zmalloc.h"

extern "C" {

// function
objectList *createObjectList(void *object) {
    int length;
    robj *o = (robj *)object;
    hashTypeIterator *hi;

    if (o->encoding == OBJ_ENCODING_HT) {
        length = dictSize((const dict *)o->ptr);
    } else {
        serverPanic("Unknown hash encoding");
    }

    objectList *result = (objectList *)zcalloc(sizeof(objectList));
    result->length = length;

    hi = hashTypeInitIterator(o);
    std::string mergedValues;
    mergedValues += VALID_MARK;
    while (hashTypeNext(hi) != C_ERR) {
        mergedValues += std::string(hashTypeCurrentFromHashTable(hi, OBJ_HASH_KEY));
        mergedValues += STRING_DELIMITER;
        mergedValues += std::string(hashTypeCurrentFromHashTable(hi, OBJ_HASH_VALUE));
        mergedValues += STRING_DELIMITER;
    }
    mergedValues += '\0';
    result->allocated_length += mergedValues.length();

    result->ptr = (char *)zcalloc(mergedValues.length());
    strcpy(result->ptr, mergedValues.c_str());

    return result;
}

bool _GetNextValidMark(const char *ptr, char delimeter, uint64_t searchOffset, uint64_t &foundOffset) {
    char *curretValue = (char *)(ptr + searchOffset);
    char mark = *(uint8_t *)curretValue;
    while (*curretValue) {
        if (mark == delimeter) {
            foundOffset = searchOffset;
            return true;
        }
        searchOffset += sizeof(delimeter);
        curretValue = (char *)(ptr + searchOffset);
        mark = *(uint8_t *)curretValue;
    }
    return false;
}

void _ParseString(const char *ptr, char delimeter, std::vector<std::string> &list) {
    uint64_t searchOffset = 0;
    uint64_t foundOffset = 0;
    bool validMarkFound = false;

    while ((validMarkFound = _GetNextValidMark(ptr, STRING_DELIMITER, searchOffset, foundOffset)) == true) {
        std::string value(ptr + searchOffset, ptr + foundOffset);
        value += '\0';
        list.push_back(value);
        searchOffset = foundOffset + 1;
    }
}

decodedObjectList *decodeRocksdbValues(const char *target, uint64_t maxOffset) {
    decodedObjectList *result = (decodedObjectList *)zcalloc(sizeof(decodedObjectList));

    uint64_t searchOffset = 0;
    uint64_t foundOffset = 0;
    bool validMarkFound = false;

    validMarkFound = _GetNextValidMark(target, VALID_MARK, searchOffset, foundOffset);
    if (validMarkFound != true) {
        serverPanic("Wrong data");
    }

    char *ptr = (char *)target + 1;
    std::vector<std::string> values;
    _ParseString(ptr, STRING_DELIMITER, values);

    result->length = values.size() / 2;
    result->fields = (char **)zrealloc(result->fields, sizeof(char *) * result->length);
    result->values = (char **)zrealloc(result->values, sizeof(char *) * result->length);

    for (int index = 0; index < result->length; index++) {
        result->fields[index] = sdsnew(values[index * 2].c_str());
        result->allocated_length += values[index * 2].length() + 1;
        result->values[index] = sdsnew(values[index * 2 + 1].c_str());
        result->allocated_length += values[index * 2 + 1].length() + 1;
    }
    return result;
}
}
