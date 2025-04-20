#ifndef ZONEDEF_H
#define ZONEDEF_H

#define ZObject_class ZClazz

#define CREATE_CLASS(clazz_name, class_parent, clazz_ctor, ...)                \
  if (!clazz_name##_class) {                                                   \
    clazz_name##_class =                                                       \
        (zclazz *)znew(ZClazz, #clazz_name "_class", class_parent##_class,     \
                       (zsize)sizeof(struct clazz_name##_class),               \
                       clazz_ctor ? zctor : NULL, clazz_ctor, NULL);           \
  }                                                                            \
  if (!clazz_name) {                                                           \
    clazz_name =                                                               \
        (zclazz *)znew(clazz_name##_class, #clazz_name, class_parent,          \
                       (zsize)sizeof(struct clazz_name), __VA_ARGS__);         \
  }

#define CREATE_ON_LOAD(constructor_priority, clazz_name, class_parent,         \
                       clazz_ctor, ...)                                        \
  static void __attribute__((constructor(constructor_priority))) on_load() {   \
    CREATE_CLASS(clazz_name, class_parent, clazz_ctor, __VA_ARGS__)            \
  }

#define CREATE_CLASS_PTR(clazz_name)                                           \
  const zclazz *clazz_name;                                                    \
  const zclazz *clazz_name##_class;

#define CREATE_INITIALIZER_BODY(constructor_priority, clazz_name,              \
                                class_parent, clazz_ctor, ...)                 \
  CREATE_CLASS_PTR(clazz_name)                                                 \
  CREATE_ON_LOAD(constructor_priority, clazz_name, class_parent, clazz_ctor,   \
                 __VA_ARGS__)
#endif