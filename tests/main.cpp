#include "../include/zmemory.h"
#include "../include/zobject.h"
#include "../include/zobject.r.h"
#include <gtest/gtest.h>
#include <stdlib.h>

class ZObjectTest : public testing::Test {
protected:
  void SetUp() override {
    a = znew(ZObject);
    b = znew(ZObject);
    aa = zclone(a);
  }
  void TearDown() override {
    zdelete(aa);
    zdelete(b);
    zdelete(a);
  }

  zobject *a;
  zobject *b;
  zobject *aa;
};

TEST_F(ZObjectTest, SizeTest) {
  EXPECT_EQ(zsizeof(a), sizeof(zobject)) << "Size of 'a' is incorrect.";
  EXPECT_EQ(zsizeof(b), sizeof(zobject)) << "Size of 'a' is incorrect.";
}

TEST_F(ZObjectTest, DifferTest) {
  // All ZObjects are similar since ZObject have no internal data
  EXPECT_FALSE(zdiffer(a, b))
      << "All ZObjects should similar since ZObject have no internal data.";
  EXPECT_FALSE(zdiffer(a, aa))
      << "All ZObjects should similar since ZObject have no internal data.";
}

TEST_F(ZObjectTest, CloneTest) {
  EXPECT_FALSE(a == aa) << "Clone 'aa' cannot have the same address as 'a'";
}

TEST_F(ZObjectTest, StoreTest) {
  FILE *f = tmpfile();
  ASSERT_NE(f, nullptr);
  zstore(a, f);
  fseek(f, 0, SEEK_SET);
  char buffer[1024] = {0};
  fread(buffer, 1, sizeof(buffer), f);
  EXPECT_STREQ(buffer, "ZObject") << "The class name should be 'ZObject'";
  fclose(f);
}
