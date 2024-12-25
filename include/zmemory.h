#ifndef ZMEMORY_H
#define ZMEMORY_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint64_t zsize;
typedef struct zobject zobject;
typedef struct zclazz zclazz;

void *zcalloc(const zsize n, const zsize size);
void zfree(void *ptr);
zobject *znew(const zclazz *clazz, ...);
void zdelete(zobject *self);

#ifdef __cplusplus
}
#endif
#endif