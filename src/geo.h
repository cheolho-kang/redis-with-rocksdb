#ifndef __GEO_H__
#define __GEO_H__

#include "server.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Structures used inside geo.c in order to represent points and array of
 * points on the earth. */
typedef struct geoPoint {
    double longitude;
    double latitude;
    double dist;
    double score;
    char *member;
} geoPoint;

typedef struct geoArray {
    struct geoPoint *array;
    size_t buckets;
    size_t used;
} geoArray;

#ifdef __cplusplus
}
#endif

#endif
