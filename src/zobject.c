#include "../include/zobject.h"
#include "../include/zmemory.h"
#include "../include/zobject.r.h"
#include <assert.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

struct zclazz {
  zsize size;
  zobject *(*ctor)(zobject *self, va_list *args);
  zobject *(*dtor)(zobject *self);
  zobject *(*clone)(const zobject *self);
  int (*differ)(const zobject *self, const zobject *other);
  zsize (*store)(const zobject *self, FILE *file);
};

void *zcalloc(const zsize n, const zsize size) { return calloc(n, size); }
void zfree(void *ptr) { return free(ptr); }

zobject *znew(const zclazz *clazz, ...) {
  const zclazz *class = clazz;
  zobject *object = zcalloc(1, class->size);
  assert(object);

  *(const zclazz **)object = clazz;
  if (clazz->ctor) {
    va_list argz;
    va_start(argz, clazz);
    object = clazz->ctor(object, &argz);
    va_end(argz);
  }
  return object;
}

void zdelete(zobject *self) {
  const zclazz **clazz = (const zclazz **)self;
  if (self && *clazz && (*clazz)->dtor)
    self = (*clazz)->dtor(self);
  zfree(self);
}

int zdiffer(const zobject *const self, const zobject *const other) {
  const zclazz *const *class = (const zclazz *const *)self;

  assert(self && *class && (*class)->differ);
  return (*class)->differ(self, other);
}

zsize zsizeof(const zobject *self) {
  const zclazz *const *class = (const zclazz *const *)self;

  assert(self && *class);
  return (*class)->size;
}

zobject *zclone(const zobject *self) {
  const zclazz *const *class = (const zclazz *const *)self;

  assert(self && *class && (*class)->clone);
  return (*class)->clone(self);
}

zsize zstore(const zobject *self, FILE *f) {
  const zclazz *const *class = (const zclazz *const *)self;

  assert(self && *class && (*class)->clone);
  return (*class)->store(self, f);
}

extern zclazz *ZObject;

static zobject *zobject_ctor(zobject *object, va_list *args) { return object; }
static zobject *zobject_dtor(zobject *object) { return object; }
static zobject *zobject_clone(const zobject *object) { return znew(ZObject); }
static int zobject_differ(const zobject *object, const zobject *other) {
  assert(object && other);
  return object != other && object->clazz != other->clazz;
}

static zsize zobject_store(const zobject *object, FILE *file) {
  return fprintf(file, "%s", "ZObject");
}

zclazz zobject_class = {sizeof(zobject), zobject_ctor,   zobject_dtor,
                        zobject_clone,   zobject_differ, zobject_store};
zclazz *ZObject = &zobject_class;
