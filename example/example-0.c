#include "../include/zclazz.r.h"
#include "../include/zmemory.h"
#include "../include/zobject.h"
#include <stdio.h>

int main() {
  // Creating a ZObject
  zobject *obj = znew(ZObject);
  printf("Created object of class: %s\n", zclassof(obj)->name);

  // Creating a subclass
  zclazz *AnyClass =
      (zclazz *)znew(ZClazz, "Any", ZObject, (zsize)sizeof(zobject), NULL);
  zobject *sub_obj = znew(AnyClass);
  printf("Created subclass object of class: %s\n", zclassof(sub_obj)->name);

  // Cloning an object
  zobject *clone_obj = zclone(obj);
  printf("Cloned object of class: %s\n", zclassof(clone_obj)->name);

  // Cleaning up
  zdelete(clone_obj);
  zdelete(sub_obj);
  zdelete(obj);

  return 0;
}