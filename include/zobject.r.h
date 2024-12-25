#ifndef ZOBJECT_R_H
#define ZOBJECT_R_H

#include "zmemory.h"

#ifdef __cplusplus
extern "C" {
#endif
struct zobject {
  zclazz *clazz;
};

#ifdef __cplusplus
}
#endif
#endif