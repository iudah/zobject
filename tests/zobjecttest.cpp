#include "../include/zobject.h"
#include "../include/zmemory.h"
#include "../include/zobject.r.h"
#include <gtest/gtest.h>
#include <stdio.h>
#include <stdlib.h>

class ZObjectTest : public testing::Test {
protected:
  void SetUp() override {
    a = znew(ZObject, NULL);
    b = znew(ZObject, NULL);
    aa = zclone(a);
  }
  void TearDown() override {
    zdelete(aa);
    zdelete(b);
    zdelete(a);
  }

  void *a;
  void *b;
  void *aa;
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
  char expected_result[256];
  snprintf(expected_result, sizeof(expected_result), "<ZObject %p>\n", a);
  EXPECT_STREQ(buffer, expected_result)
      << "The class name should be 'ZObject', got " << buffer;
  fclose(f);
}
