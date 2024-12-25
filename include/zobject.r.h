#ifndef ZOBJECT_R_H
#define ZOBJECT_R_H

#include "zmemory.h"
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

struct zobject {
  zclazz *clazz;
};

zobject *zctor(zobject *object, va_list *argp);
zobject *zdtor(zobject *self);

#ifdef __cplusplus
}
#endif
#endif