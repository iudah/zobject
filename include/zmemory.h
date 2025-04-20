#ifndef ZMEMORY_H
#define ZMEMORY_H

#include <stdint.h>

#include "zclazz.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Type definition for object sizes.
 */
typedef uint64_t zsize;

/**
 * Forward declaration of a generic object.
 */
typedef struct zobject zobject;

/**
 * Creates a new object of the specified class.
 * @param clazz Pointer to the object's class.
 * @return Pointer to the newly created object.
 */
void *znew(const zclazz *clazz, ...) __attribute__((__sentinel__));

/**
 * Deletes an object, invoking its destructor and freeing memory.
 * @param self Pointer to the object to be deleted.
 */
void zdelete(void *self);

#define LOG(fmt, ...) fprintf(stderr, "[LOG]: " fmt "\n", ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif