#include <cstdio>
#include <gtest/gtest.h>
#include <stdlib.h>

extern "C" {
#include "../include/zclazz.r.h"
#include "../include/zmemory.h"
#include "../include/zobject.h"

static int any_differ(const zobject *object, const zobject *other) { return 0; }
}

class ZSubclassTest : public testing::Test {
protected:
  void SetUp() override {
    a = znew(ZObject, NULL);

    Any = (zclazz *)znew(ZClazz, "Any", ZObject, zsizeof(a), zdiffer,
                         any_differ, NULL);
    b = znew(Any, NULL);
  }

  void TearDown() override {
    zdelete(b);
    zdelete(a);
  }

  void *a;
  void *b;
  zclazz *Any;
};

TEST_F(ZSubclassTest, ClassNamesTest) {
  char buffer[1024] = {0};
  FILE *f = tmpfile();
  ASSERT_NE(f, nullptr);

  zstore(a, f);
  zstore(b, f);
  zstore((zobject *)Any, f);

  fseek(f, 0, SEEK_SET);
  fread(buffer, 1, sizeof(buffer), f);
  fclose(f);

  char buffer_[1024];
  snprintf(buffer_, sizeof(buffer_), "<%s %p>\n<%s %p>\n<%s %p>\n",
           ((zclazz *)zclassof(a))->name, a, ((zclazz *)zclassof(b))->name, b,
           ((zclazz *)zclassof((zobject *)Any))->name, Any);

  EXPECT_STREQ(buffer, buffer_) << "The output should be: " << buffer_;
}

TEST_F(ZSubclassTest, DifferTest) {
  EXPECT_FALSE(zdiffer(a, a))
      << "An object should not differ from itself (a!=a).";
  EXPECT_FALSE(zdiffer(b, b))
      << "An object of class 'Any' should not differ from itself (b!=b).";
  EXPECT_TRUE(zdiffer(a, b))
      << "Objects of different class should differ ('Object' vs 'Any').";
  EXPECT_FALSE(zdiffer(b, a)) << "An object of class 'Any' should not differ "
                                 "from object of another class.";

  EXPECT_EQ(zdiffer(a, a), zdiffer(b, b));
  EXPECT_NE(zdiffer(a, b), zdiffer(b, a))
      << "Matching of an object of class 'Any' to other classes should not be "
         "commutative.";
}