#ifndef ZMEMORY_H
#define ZMEMORY_H

#include "zclazz.h"
#include <stdint.h>

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
 * Allocates memory for an array of objects.
 * @param n Number of objects.
 * @param size Size of each object.
 * @return Pointer to the allocated memory.
 */
void *zcalloc(const zsize n, const zsize size);

/**
 * Frees allocated memory.
 * @param ptr Pointer to the memory to be freed.
 */
void zfree(void *ptr);

/**
 * Creates a new object of the specified class.
 * @param clazz Pointer to the object's class.
 * @return Pointer to the newly created object.
 */
zobject *znew(const zclazz *clazz, ...);

/**
 * Deletes an object, invoking its destructor and freeing memory.
 * @param self Pointer to the object to be deleted.
 */
void zdelete(zobject *self);

#ifdef __cplusplus
}
#endif

#endif