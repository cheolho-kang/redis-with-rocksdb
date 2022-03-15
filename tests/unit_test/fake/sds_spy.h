#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef char *sds;

sds sdsnew(const char *init);
void sdsfree(sds s);

#ifdef __cplusplus
}
#endif
