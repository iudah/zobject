#include "../include/zclazz.r.h"
#include "../include/zmemory.h"
#include "../include/zobject.h"
#include <stdio.h>
#include <string.h>

// Define the Person struct
typedef struct {
  zobject base; // Base object
  char *name;   // Name of the person
  int age;      // Age of the person
} Person;

// Person constructor
zobject *Person_ctor(zobject *self, va_list *args) {
  Person *person = (Person *)self;
  const char *name = va_arg(*args, const char *);
  int age = va_arg(*args, int);

  person->name = strdup(name); // Copy the name
  person->age = age;

  return self;
}

// Person destructor
zobject *Person_dtor(zobject *self) {
  Person *person = (Person *)self;
  free(person->name); // Free allocated memory for the name
  return self;
}

// Override clone method for Person
zobject *Person_clone(const zobject *self) {
  const Person *person = (const Person *)self;
  return znew(zclassof(self), person->name, person->age, NULL);
}

// Override differ method for Person
int Person_differ(const zobject *self, const zobject *other) {
  const Person *p1 = (const Person *)self;
  const Person *p2 = (const Person *)other;

  if (zclassof(self) != zclassof(other))
    return 1; // Check class equality
  if (strcmp(p1->name, p2->name) != 0)
    return 1; // Compare names
  if (p1->age != p2->age)
    return 1; // Compare ages

  return 0;
}

int main() {
  // Create a Person class
  zclazz *PersonClass = (zclazz *)znew(
      ZClazz, "Person", ZObject, (zsize)sizeof(Person), zctor, Person_ctor,
      zdtor, Person_dtor, zclone, Person_clone, zdiffer, Person_differ, NULL);

  // Create a Person instance
  Person *p1 = (Person *)znew(PersonClass, "Alice", 30, NULL);
  printf("Person 1: Name = %s, Age = %d\n", p1->name, p1->age);

  // Clone the Person instance
  Person *p2 = (Person *)zclone((zobject *)p1);
  printf("Person 2 (Clone): Name = %s, Age = %d\n", p2->name, p2->age);

  // Compare the original and the clone
  printf("Are Person 1 and Person 2 different? %s\n",
         zdiffer((zobject *)p1, (zobject *)p2) ? "Yes" : "No");

  // Modify the clone
  free(p2->name);
  p2->name = strdup("Bob");
  p2->age = 25;
  printf("Person 2 (Modified): Name = %s, Age = %d\n", p2->name, p2->age);

  // Compare again after modification
  printf("Are Person 1 and Person 2 different now? %s\n",
         zdiffer((zobject *)p1, (zobject *)p2) ? "Yes" : "No");

  // Clean up
  zdelete((zobject *)p1);
  zdelete((zobject *)p2);
  zdelete((zobject *)PersonClass);

  return 0;
}