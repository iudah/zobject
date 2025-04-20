#include <stdio.h>

#include "../include/zclazz.r.h"
#include "../include/zmemory.h"
#include "../include/zobject.h"

int main() {
  // Creating a ZObject
  zobject *obj = znew(ZObject, NULL);
  printf("Created object of class: %s\n", ((zclazz *)zclassof(obj))->name);

  // Creating a subclass
  zclazz *AnyClass =
      (zclazz *)znew(ZClazz, "Any", ZObject, (zsize)sizeof(zobject), NULL);
  zobject *sub_obj = znew(AnyClass, NULL);
  printf("Created subclass object of class: %s\n",
         ((zclazz *)zclassof(sub_obj))->name);

  // Cloning an object
  zobject *clone_obj = zclone(obj);
  printf("Cloned object of class: %s\n", ((zclazz *)zclassof(clone_obj))->name);

  // Cleaning up
  zdelete(clone_obj);
  zdelete(sub_obj);
  zdelete(obj);

  return 0;
}