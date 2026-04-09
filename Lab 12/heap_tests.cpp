#include <cstdlib>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "heap.h"

using namespace std;
using namespace testing;

namespace {
  TEST(MinHeapInsert, BasicInserts) {
    MinHeap heap;
    TrainStation t1;
    t1.id = 1;
    t1.name = "dummy1";
    t1.ridership = 4;
    heap.insert(t1);
    TrainStation inHeap = heap.peek();
    EXPECT_THAT(inHeap.ridership, Eq(4));

    TrainStation t2;
    t2.id = 2;
    t2.name = "dummy2";
    t2.ridership = 10;
    heap.insert(t2);
    inHeap = heap.peek();
    EXPECT_THAT(inHeap.ridership, Eq(4));

    TrainStation t3;
    t3.id = 3;
    t3.name = "dummy3";
    t3.ridership = 2;
    heap.insert(t3);
    inHeap = heap.peek();
    EXPECT_THAT(inHeap.ridership, Eq(2));

    TrainStation t4;
    t4.id = 4;
    t4.name = "dummy4";
    t4.ridership = 11;
    heap.insert(t4);
    inHeap = heap.peek();
    EXPECT_THAT(inHeap.ridership, Eq(2));

    TrainStation t5;
    t5.id = 5;
    t5.name = "dummy5";
    t5.ridership = 7;
    heap.insert(t5);
    inHeap = heap.peek();
    EXPECT_THAT(inHeap.ridership, Eq(2));

    TrainStation t6;
    t6.id = 6;
    t6.name = "dummy6";
    t6.ridership = 8;
    heap.insert(t6);
    inHeap = heap.peek();
    EXPECT_THAT(inHeap.ridership, Eq(2));

    TrainStation t7;
    t7.id = 7;
    t7.name = "dummy7";
    t7.ridership = 1;
    heap.insert(t7);
    inHeap = heap.peek();
    EXPECT_THAT(inHeap.ridership, Eq(1));

    TrainStation t8;
    t8.id = 8;
    t8.name = "dummy8";
    t8.ridership = 0;
    heap.insert(t8);
    inHeap = heap.peek();
    EXPECT_THAT(inHeap.ridership, Eq(0));

    TrainStation t9;
    t9.id = 9;
    t9.name = "dummy9";
    t9.ridership = 23;
    heap.insert(t9);
    inHeap = heap.peek();
    EXPECT_THAT(inHeap.ridership, Eq(0));

    EXPECT_THAT(heap.size(), Eq(9));
  }

  TEST(MinHeapRemove, Removes) {
    MinHeap heap;
    TrainStation t1;
    t1.id = 1;
    t1.name = "dummy1";
    t1.ridership = 4;
    heap.insert(t1);

    TrainStation t2;
    t2.id = 2;
    t2.name = "dummy2";
    t2.ridership = 10;
    heap.insert(t2);

    TrainStation t3;
    t3.id = 3;
    t3.name = "dummy3";
    t3.ridership = 2;
    heap.insert(t3);

    TrainStation t4;
    t4.id = 4;
    t4.name = "dummy4";
    t4.ridership = 11;
    heap.insert(t4);

    TrainStation t5;
    t5.id = 5;
    t5.name = "dummy5";
    t5.ridership = 7;
    heap.insert(t5);

    TrainStation t6;
    t6.id = 6;
    t6.name = "dummy6";
    t6.ridership = 8;
    heap.insert(t6);

    TrainStation t7;
    t7.id = 7;
    t7.name = "dummy7";
    t7.ridership = 1;
    heap.insert(t7);

    TrainStation t8;
    t8.id = 8;
    t8.name = "dummy8";
    t8.ridership = 0;
    heap.insert(t8);

    TrainStation t9;
    t9.id = 9;
    t9.name = "dummy9";
    t9.ridership = 23;
    heap.insert(t9);

    EXPECT_THAT(heap.remove().ridership, Eq(0));
    EXPECT_THAT(heap.size(), Eq(8));
    EXPECT_THAT(heap.remove().ridership, Eq(1));
    EXPECT_THAT(heap.remove().ridership, Eq(2));
    EXPECT_THAT(heap.remove().ridership, Eq(4));
    EXPECT_THAT(heap.remove().ridership, Eq(7));
    EXPECT_THAT(heap.remove().ridership, Eq(8));
    EXPECT_THAT(heap.remove().ridership, Eq(10));
    EXPECT_THAT(heap.remove().ridership, Eq(11));
    EXPECT_THAT(heap.remove().ridership, Eq(23));
    EXPECT_THAT(heap.size(), Eq(0));
  }
};  // namespace
