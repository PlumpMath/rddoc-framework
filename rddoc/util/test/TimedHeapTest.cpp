/*
 * Copyright (C) 2017, Yeolar
 */

#include "rddoc/util/TimedHeap.h"
#include <gtest/gtest.h>

using namespace rdd;

TEST(TimedHeap, push_pop) {
  TimedHeap<int> heap;
  uint64_t t = timestampNow();

  int i1 = 1;
  uint64_t t1 = t + 1;
  auto timeout1 = Timeout<int>(&i1, t1);
  int i2 = 2;
  uint64_t t2 = t + 2;
  auto timeout2 = Timeout<int>(&i2, t2);
  int i3 = 3;
  uint64_t t3 = t + 3;
  auto timeout3 = Timeout<int>(&i3, t3);
  int i4 = 4;
  uint64_t t4 = t + 4;
  auto timeout4 = Timeout<int>(&i4, t4);

  heap.push(timeout1);
  heap.push(timeout2);
  heap.push(timeout3);
  heap.push(timeout4);

  auto timeoutA = heap.pop(t1);
  EXPECT_EQ(i1, *timeoutA.data);
  EXPECT_EQ(t1, timeoutA.deadline);
  auto timeoutB = heap.pop(t2);
  EXPECT_EQ(i2, *timeoutB.data);
  EXPECT_EQ(t2, timeoutB.deadline);
  auto timeoutC = heap.pop(t3);
  EXPECT_EQ(i3, *timeoutC.data);
  EXPECT_EQ(t3, timeoutC.deadline);
  auto timeoutD = heap.pop(t4);
  EXPECT_EQ(i4, *timeoutD.data);
  EXPECT_EQ(t4, timeoutD.deadline);
  auto timeoutE = heap.pop(t4);
  EXPECT_EQ(nullptr, timeoutE.data);
}