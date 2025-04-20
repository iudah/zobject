#ifndef ZCLAZZ_R_H
#define ZCLAZZ_R_H

#include <stdarg.h>
#include <stdio.h>

#include "zobject.r.h"

/**
 * Forward declaration of the zclazz structure.
 */
typedef struct zclazz zclazz;

extern zclazz *ZClazz;

/**
 * Class structure definition.
 */
struct zclazz {
  zobject _;     /**< Base object. */
  char *name;    /**< Name of the class. */
  zclazz *super; /**< Pointer to the superclass. */
  zsize size;    /**< Size of the class instances. */
  zobject *(*ctor)(zobject *self, va_list *args); /**< Constructor. */
  zobject *(*dtor)(zobject *self);                /**< Destructor. */
  zobject *(*clone)(const zobject *self);         /**< Clone method. */
  int (*differ)(const zobject *self,
                const zobject *other);             /**< Differ method. */
  zsize (*store)(const zobject *self, FILE *file); /**< Store method. */
};

#endif