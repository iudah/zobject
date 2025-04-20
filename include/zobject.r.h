#ifndef ZOBJECT_R_H
#define ZOBJECT_R_H

#include <stdarg.h>

#include "zmemory.h"

#ifdef __cplusplus
extern "C" {
#endif

struct zobject {
  zclazz *clazz;
};

zobject *zctor(zobject *object, va_list *argp);
zobject *zdtor(zobject *self);
zobject *zsuper_ctor(zclazz *clazz, zobject *object, va_list *argp);
zobject *zsuper_dtor(zclazz *clazz, zobject *object);

#ifdef __cplusplus
}
#endif
#endif