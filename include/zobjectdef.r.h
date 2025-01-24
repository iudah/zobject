#ifndef ZOBJECTDEF_H
#define ZOBJECTDEF_H

#include <inttypes.h>
#include <stdlib.h>

#define ZObject_class ZClazz
#define zobject_class zclazz

#define Z_DEFINE_CLASS_INSTANCE(clazz_name, class_parent, clazz_ctor, ...)     \
  if (!clazz_name##_class) {                                                   \
    clazz_name##_class =                                                       \
        (zclazz *)znew(ZClazz, #clazz_name "_class", class_parent##_class,     \
                       (zsize)sizeof(struct clazz_name##_class),               \
                       clazz_ctor ? zctor : NULL, clazz_ctor, NULL);           \
  }                                                                            \
  if (!clazz_name) {                                                           \
    clazz_name =                                                               \
        (zclazz *)znew(clazz_name##_class, #clazz_name, class_parent,          \
                       (zsize)sizeof(struct clazz_name), ##__VA_ARGS__);       \
  }

#define Z_INIT_CLASS_ON_LOAD(constructor_priority, clazz_name, class_parent,   \
                             clazz_ctor, ...)                                  \
  static void __attribute__((constructor(constructor_priority))) on_load() {   \
    Z_DEFINE_CLASS_INSTANCE(clazz_name, class_parent, clazz_ctor,              \
                            ##__VA_ARGS__)                                     \
  }

#define Z_DECLARE_CLASS_PTRS(clazz_name)                                       \
  const zclazz *clazz_name = NULL;                                             \
  const zclazz *clazz_name##_class = NULL;

#define Z_INIT_CLASS_WITH_PRIORITIES(constructor_priority, clazz_name,         \
                                     class_parent, clazz_ctor, ...)            \
  Z_DECLARE_CLASS_PTRS(clazz_name)                                             \
  Z_INIT_CLASS_ON_LOAD(constructor_priority, clazz_name, class_parent,         \
                       clazz_ctor, ##__VA_ARGS__)

#define Z_SELECTOR_PAIR(classname, fnname)                                     \
  (voidf) classname##_##fnname, (voidf) & class->fnname

#define Z_DEFINE_CLASS_CONSTRUCTOR(classname, classtype, ...)                  \
  static classtype##_class *classtype##_class_constructor(                     \
      classtype##_class *class, va_list *argp) {                               \
    zsuper_ctor((zclazz *)classname##_class, (zobject *)class, argp);          \
    typedef void (*voidf)(void);                                               \
    voidf selectors[] = {__VA_ARGS__};                                         \
    zsize selectors_length = sizeof(selectors) / (2 * sizeof(*selectors));     \
    va_list arg ; va_copy(arg,*argp);                                                       \
    voidf selector;                                                            \
    while ((selector = va_arg(arg, voidf))) {                                  \
      voidf method = va_arg(arg, voidf);                                       \
      zsize i;                                                                 \
      for (i = 0; i < selectors_length; i++) {                                 \
        if (selectors[i * 2] == selector)                                      \
          memcpy(selectors[i * 2 + 1], &method, sizeof(method));               \
      }                                                                        \
      if (i == selectors_length) {                                             \
        LOG("%s (%p, %p) ignored\n", __FUNCTION__, selector,       \
            method);                                         \
      }                                                                        \
    }    va_end(arg);                                                                      \
    return class;                                                              \
  }

#define Z_DECLARE_CLASS(parenttype, classname, type, members, methods)         \
  extern const zclazz *classname##_class;                                      \
  typedef struct classname##_class type##_class;                               \
                                                                               \
  struct classname {                                                           \
    parenttype _;                                                              \
    members                                                                    \
  };                                                                           \
                                                                               \
  struct classname##_class {                                                   \
    parenttype##_class _;                                                      \
    methods                                                                    \
  };

#define Z_DECLARE_CLASS_METHOD(returntype, classtype, fnname, ...)             \
  returntype (*fnname)(classtype * self, ##__VA_ARGS__);

#define Z_DECLARE_CLASS_TYPE(clazz, type)                                      \
  extern const zclazz *clazz;                                                  \
  typedef struct clazz type;

#define Z_SUPER_CALL(obj, fn, ...)                                             \
  {                                                                            \
    zclazz *__##obj##_##fn##_variable__ = ((zobject *)obj)->clazz;             \
    ((zobject *)obj)->clazz = (__##obj##_##fn##_variable__)->super;            \
    fn((void *)obj, ##__VA_ARGS__);                                            \
    ((zobject *)obj)->clazz = __##obj##_##fn##_variable__;                     \
  }

#endif