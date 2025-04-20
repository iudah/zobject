#include "../include/zobject.h"

#include <assert.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/cdefs.h>
#include <zot.h>

#include "../include/zclazz.r.h"
#include "../include/zmemory.h"
#include "../include/zobject.h"
#include "../include/zobject.r.h"

typedef uint64_t zsize;
typedef struct zclazz zclazz;

void *zclassof(const void *self) {
  const zobject *object = self;
  assert(object && object->clazz);
  return object->clazz;
}

zclazz *zsuper(zclazz *clazz) {
  assert(clazz && clazz->super);
  return clazz->super;
}

zobject *zsuper_ctor(zclazz *clazz, zobject *object, va_list *argp) {
  const zclazz *superclass = zsuper(clazz);
  assert(object && superclass);
  return superclass->ctor(object, argp);
}

zobject *zctor(zobject *object, va_list *argp) {
  const zclazz *class = zclassof(object);
  assert(class->ctor);
  return class->ctor(object, argp);
}

void *znew(const zclazz *clazz, ...) {
  const zclazz *class = clazz;
  zobject *object;
  va_list arg;

  assert(class && class->size);
  object = zcalloc(1, class->size);

  assert(object);
  object->clazz = (zclazz *)clazz;

  va_start(arg, clazz);
  object = zctor(object, &arg);
  va_end(arg);

  return object;
}

zobject *zsuper_dtor(zclazz *clazz, zobject *object) {
  const zclazz *superclass = zsuper(clazz);
  assert(object && superclass);
  return superclass->dtor(object);
}

zobject *zdtor(zobject *self) {
  const zclazz *class = zclassof(self);
  assert(class->dtor);
  return class->dtor(self);
}

void zdelete(void *object) {
  zobject *self = object;
  const zclazz **clazz = (const zclazz **)self;
  if (self && *clazz && (*clazz)->dtor) self = zdtor(self);
  zfree(self);
}

int zdiffer(const void *const self_object, const void *const other_object) {
  const zobject *const self = self_object;
  const zobject *const other = other_object;
  const zclazz *const *class = (const zclazz *const *)self;

  assert(self && *class && (*class)->differ);
  return (*class)->differ(self, other);
}

void *zclone(const void *self_object) {
  const zobject *self = self_object;
  const zclazz *const *class = (const zclazz *const *)self;

  assert(self && *class && (*class)->clone);
  return (*class)->clone(self);
}

zsize zstore(const void *object, FILE *f) {
  const zobject *self = object;
  const zclazz *const *class = (const zclazz *const *)self;

  assert(self && *class && (*class)->store);
  return (*class)->store(self, f);
}

zsize zsizeof(const void *self) {
  const zobject *object = self;
  const zclazz *class = zclassof(object);
  return class->size;
}

static zobject *zobject_ctor(zobject *object, va_list *argp) { return object; }

static zobject *zobject_dtor(zobject *object) { return object; }

static zobject *zobject_clone(const zobject *object) {
  return znew(ZObject, NULL);
}

static int zobject_differ(const zobject *object, const zobject *other) {
  return object != other && object->clazz != other->clazz;
}

static zsize zobject_store(const zobject *object, FILE *file) {
  const zclazz *class = zclassof(object);
  return fprintf(file, "<%s %p>\n", class->name, object);
}

static zobject *zclazz_dtor(zobject *self) {
  zclazz *class = (zclazz *)self;
  fprintf(stdout, "%s: cannot destroy class\n", class->name);
  return NULL;
}

static zobject *zclazz_ctor(zobject *self, va_list *argp) {
  zclazz *class = (zclazz *)self;

  class->name = va_arg(*argp, char *);
  class->super = va_arg(*argp, zclazz *);
  class->size = va_arg(*argp, zsize);

  if (!class->super) {
    LOG("class->super is NULL");
    abort();
  }

  assert(class->super);

  const zsize offset = offsetof(zclazz, ctor);

  memcpy((char *)class + offset, (char *)class->super + offset,
         zsizeof((zobject *)class->super) - offset);

  // overwrite class methods
  typedef void (*voidf)();
  voidf selector;
  va_list arg;
  va_copy(arg, *argp);
  zsize arg_i = 0;
  while ((selector = va_arg(arg, voidf))) {
    voidf method = va_arg(arg, voidf);

    if (selector == (voidf)zctor) {
      memcpy(&class->ctor, &method, sizeof(method));
    } else if (selector == (voidf)zdtor) {
      *(voidf *)&class->dtor = method;
    } else if (selector == (voidf)zdiffer) {
      *(voidf *)&class->differ = method;
    } else if (selector == (voidf)zstore) {
      *(voidf *)&class->store = method;
    } else if (selector == (voidf)zclone) {
      *(voidf *)&class->clone = method;
    } else {
      // fprintf(stderr, "arg#%" PRIu64 " (%p, %p) ignored\n", arg_i, selector,
      //         method);
    }
    ++arg_i;
  }
  va_end(arg);
  return (zobject *)class;
}

zclazz z_object_class[] = {{{z_object_class + 1},
                            "ZObject",
                            z_object_class,
                            sizeof(zobject),
                            zobject_ctor,
                            zobject_dtor,
                            zobject_clone,
                            zobject_differ,
                            zobject_store},
                           {{z_object_class + 1},
                            "ZClazz",
                            z_object_class,
                            sizeof(zclazz),
                            zclazz_ctor,
                            zclazz_dtor,
                            zobject_clone,
                            zobject_differ,
                            zobject_store}};

zclazz *ZObject = z_object_class;
zclazz *ZClazz = z_object_class + 1;

static void __attribute__((constructor(100))) on_load() {
  if (ZObject) ZObject = z_object_class;
  if (ZClazz) ZClazz = z_object_class + 1;
}