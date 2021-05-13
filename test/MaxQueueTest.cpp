#include "CppUTest/TestHarness.h"

extern "C" {
#include "MaxQueue.h"
}

// -----------------------------------------------------------------------------

static void fake() {};

TEST_GROUP(MaxQueue) {
};

TEST(MaxQueue, One) {
  MaxQueue queue1 = MaxQueue_Create(10);
  MaxQueue_Add(&queue1, 10);
  MaxQueue_Add(&queue1, 20);
  MaxQueue_Add(&queue1, 30);
  MaxQueue_Add(&queue1, 60);
  MaxQueue_Add(&queue1, 50);
  CHECK_EQUAL(60, MaxQueue_Max(&queue1));
  CHECK_EQUAL(5, *MaxQueue_Index(&queue1));
  CHECK_EQUAL(10, MaxQueue_Values(&queue1)[0]);
}

TEST(MaxQueue, Two) {
  MaxQueue queue1 = MaxQueue_Create(3);
  MaxQueue_Add(&queue1, 10);
  MaxQueue_Add(&queue1, 40);
  MaxQueue_Add(&queue1, 30);
  MaxQueue queue2 = MaxQueue_Create(10);
  MaxQueue_Add(&queue2, 15);
  MaxQueue_Add(&queue2, 25);
  MaxQueue_Add(&queue2, 35);
  MaxQueue_Add(&queue2, 65);
  MaxQueue_Add(&queue2, 55);
  CHECK_EQUAL(40, MaxQueue_Max(&queue1));
  CHECK_EQUAL(3, *MaxQueue_Index(&queue1));
  CHECK_EQUAL(10, MaxQueue_Values(&queue1)[0]);
  CHECK_EQUAL(65, MaxQueue_Max(&queue2));
  CHECK_EQUAL(5, *MaxQueue_Index(&queue2));
  CHECK_EQUAL(15, MaxQueue_Values(&queue2)[0]);
  CHECK_EQUAL(55, MaxQueue_Values(&queue2)[4]);
}

TEST(MaxQueue, Reset) {
  MaxQueue queue1 = MaxQueue_Create(3);
  MaxQueue_Add(&queue1, 10);
  MaxQueue_Add(&queue1, 20);
  MaxQueue_Add(&queue1, 30);
  MaxQueue_Reset(&queue1);
  CHECK_EQUAL(-1, MaxQueue_Max(&queue1));
  CHECK_EQUAL(0, *MaxQueue_Index(&queue1));
  CHECK_EQUAL(0, MaxQueue_Values(&queue1)[0]);
}

TEST(MaxQueue, PostReset) {
  MaxQueue queue1 = MaxQueue_Create(3);
  MaxQueue_Add(&queue1, 5);
  MaxQueue_Add(&queue1, 15);
  MaxQueue_Add(&queue1, 25);
  MaxQueue_Reset(&queue1);
  MaxQueue_Add(&queue1, 10);
  MaxQueue_Add(&queue1, 20);
  MaxQueue_Add(&queue1, 30);
  CHECK_EQUAL(30, MaxQueue_Max(&queue1));
  CHECK_EQUAL(3, *MaxQueue_Index(&queue1));
  CHECK_EQUAL(10, MaxQueue_Values(&queue1)[0]);
}

TEST(MaxQueue, Overflow) {
  MaxQueue queue1 = MaxQueue_Create(3);
  MaxQueue_Add(&queue1, 70);
  MaxQueue_Add(&queue1, 20);
  MaxQueue_Add(&queue1, 30);
  MaxQueue_Add(&queue1, 60);
  MaxQueue_Add(&queue1, 50);
  CHECK_EQUAL(30, MaxQueue_Values(&queue1)[0]);
  CHECK_EQUAL(3, *MaxQueue_Index(&queue1));
  CHECK_EQUAL(70, MaxQueue_Max(&queue1));
}