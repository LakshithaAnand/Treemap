#include <gtest/gtest.h>

#include "treemap.h"

TEST(Treemap, Empty) {
  Treemap<int, int> map;

  /* Should be fully empty */
  EXPECT_EQ(map.Empty(), true);
  EXPECT_EQ(map.Size(), 0);
  EXPECT_THROW(map.Get(42), std::exception);
}

TEST(Treemap, OneKey) {
  Treemap<int, char> map;

  /* Test some insertion */
  map.Insert(23, 'A');
  map.Insert(42, 'B');
  EXPECT_EQ(map.Empty(), false);
  EXPECT_EQ(map.Size(), 2);
  EXPECT_EQ(map.MinKey(), 23);
  EXPECT_EQ(map.MaxKey(), 42);
  EXPECT_EQ(map.Get(23), 'A');
}

TEST(Treemap, Remove) {
  Treemap<int, char> map;

  /* Test some removal */
  map.Insert(23, 'A');
  map.Insert(42, 'B');
  map.Remove(23);
  EXPECT_EQ(map.Empty(), false);
  EXPECT_EQ(map.Size(), 1);
  EXPECT_EQ(map.MaxKey(), 42);
  EXPECT_EQ(map.Get(42), 'B');
}

TEST(Treemap, Contains) {
  Treemap<int, char> map;
  map.Insert(23, 'A');
  map.Insert(42, 'B');
  EXPECT_EQ(map.ContainsKey(23), true);
  EXPECT_EQ(map.ContainsKey(56), false);
  EXPECT_EQ(map.ContainsValue('B'), true);
  EXPECT_EQ(map.ContainsValue(23), false);
}

TEST(Treemap, FloorCeil) {
  Treemap<int, char> map;
  map.Insert(51, 'A');
  map.Insert(43, 'B');
  map.Insert(93, 'C');
  map.Insert(18, 'D');
  map.Insert(54, 'E');
  map.Insert(99, 'F');
  map.Insert(2, 'G');
  map.Insert(42, 'H');
  map.Insert(74, 'I');
  EXPECT_EQ(map.FloorKey(25), 18);
  EXPECT_EQ(map.CeilKey(25), 42);
  EXPECT_THROW(map.CeilKey(100), std::exception);
  EXPECT_THROW(map.FloorKey(1), std::exception);
}


TEST(Treemap, Exceptions) {
  Treemap<int, char> map;

  EXPECT_THROW(map.Get(42), std::exception);
  map.Insert(51, 'A');
  map.Insert(43, 'B');
  map.Insert(93, 'C');
  map.Insert(18, 'D');
  map.Insert(54, 'E');
  EXPECT_THROW(map.Insert(18, 'D'), std::exception);
  EXPECT_THROW(map.Remove(25), std::exception);
  EXPECT_THROW(map.Get(52), std::exception);
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

