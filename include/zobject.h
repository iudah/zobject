#ifndef ZOBJECT_H
#define ZOBJECT_H

#include "zclazz.h"
#include "zmemory.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Forward declaration of the zobject structure.
 */
typedef struct zobject zobject;

/**
 * Global reference to the ZObject class.
 */
extern zclazz *ZObject;

/**
 * Clones the given object.
 * @param self Pointer to the object to be cloned.
 * @return Pointer to the cloned object.
 */
void *zclone(const void *self);

/**
 * Returns the size of the given object.
 * @param object Pointer to the object.
 * @return Size of the object.
 */
zsize zsizeof(const void *object);

/**
 * Compares two objects for differences.
 * @param self Pointer to the first object.
 * @param other Pointer to the second object.
 * @return Non-zero if objects are different, 0 otherwise.
 */
int zdiffer(const void *const self, const void *const other);

/**
 * Stores the object's state in a file.
 * @param self Pointer to the object.
 * @param f File pointer to write to.
 * @return Number of bytes written.
 */
zsize zstore(const void *self, FILE *f);

/**
 * Gets the class of the given object.
 * @param object Pointer to the object.
 * @return Pointer to the class of the object.
 */
void *zclassof(const void *object);

#define Z_DECLARE_CLASS_TYPE(clazz, type)                                      \
  extern const zclazz *clazz;                                                  \
  typedef struct clazz type;

#ifdef __cplusplus
}
#endif

#endif