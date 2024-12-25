#ifndef ZOBJECT_H
#define ZOBJECT_H

#include "zmemory.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct zobject zobject;
extern zclazz *ZObject;

zobject *zclone(const zobject *self);
zsize zsizeof(const zobject *object);
int zdiffer(const zobject *const self, const zobject *const other);
zsize zstore(const zobject *self, FILE *f);

#ifdef __cplusplus
}
#endif
#endif