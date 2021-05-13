#include "CppUTest/TestHarness.h"

extern "C" {
#include "Peak.h"
#include "MaxQueue.h"
}

// -----------------------------------------------------------------------------

int values[90];
unsigned int nonlocal_index = 0;

static void fake() {};

TEST_GROUP(Peak) {
};

TEST(Peak, AddValue) {
  MaxQueue queue = Peak_Create(5, 3);
  Peak_AddValue(&queue, 100);
  CHECK_EQUAL(100, Peak_GetMaximum(&queue));
  Peak_Destroy(&queue);
}

TEST(Peak, GetSeriesMaximum) {
  MaxQueue queue = Peak_Create(5, 3);
  Peak_AddValue(&queue, 10);
  Peak_AddValue(&queue, 50);
  Peak_AddValue(&queue, 40);
  CHECK_EQUAL(50, Peak_GetMaximum(&queue));
  Peak_Destroy(&queue);
}

TEST(Peak, GetSeriesAllNegativeMaximum) {
  MaxQueue queue = Peak_Create(5, 3);
  Peak_AddValue(&queue, -10);
  Peak_AddValue(&queue, -50);
  Peak_AddValue(&queue, -40);
  CHECK_EQUAL(50, Peak_GetMaximum(&queue));
  Peak_Destroy(&queue);
}

TEST(Peak, GetNegativesMaximum) {
  MaxQueue queue = Peak_Create(5, 3);
  Peak_AddValue(&queue, 10);
  Peak_AddValue(&queue, -50);
  Peak_AddValue(&queue, 40);
  CHECK_EQUAL(50, Peak_GetMaximum(&queue));
  Peak_Destroy(&queue);
}

TEST(Peak, OverflowHidden) {
  MaxQueue queue = Peak_Create(3, 3);
  Peak_AddValue(&queue, 80);
  Peak_AddValue(&queue, 50);
  Peak_AddValue(&queue, 40);
  Peak_AddValue(&queue, 30);
  CHECK_EQUAL(80, Peak_GetMaximum(&queue));
  Peak_Destroy(&queue);
}

TEST(Peak, IsDecreasing) {
  MaxQueue queue = Peak_Create(5, 4);
  Peak_AddValue(&queue, 80);
  Peak_AddValue(&queue, 50);
  Peak_AddValue(&queue, 40);
  Peak_AddValue(&queue, 30);
  Peak_AddValue(&queue, 20);
  CHECK_EQUAL(1, Peak_IsDecreasing(&queue));
  Peak_Destroy(&queue);
}

TEST(Peak, IsDecreasingNegative) {
  MaxQueue queue = Peak_Create(5, 4);
  Peak_AddValue(&queue, -20);
  Peak_AddValue(&queue, -30);
  Peak_AddValue(&queue, -40);
  Peak_AddValue(&queue, -50);
  Peak_AddValue(&queue, -60);
  CHECK_EQUAL(1, Peak_IsDecreasing(&queue));
  Peak_Destroy(&queue);
}

TEST(Peak, IsDecreasingMixed) {
  MaxQueue queue = Peak_Create(5, 4);
  Peak_AddValue(&queue, 10);
  Peak_AddValue(&queue, 5);
  Peak_AddValue(&queue, 0);
  Peak_AddValue(&queue, -5);
  Peak_AddValue(&queue, -100);
  CHECK_EQUAL(1, Peak_IsDecreasing(&queue));
  Peak_Destroy(&queue);
}

TEST(Peak, IsNotDecreasing) {
  MaxQueue queue = Peak_Create(5, 4);
  Peak_AddValue(&queue, 20);
  Peak_AddValue(&queue, 50);
  Peak_AddValue(&queue, 40);
  Peak_AddValue(&queue, 30);
  CHECK_EQUAL(0, Peak_IsDecreasing(&queue));
  Peak_Destroy(&queue);
}

TEST(Peak, PeakAndQueue) {
  MaxQueue queue = Peak_Create(5, 4);
  MaxQueue queue1 = MaxQueue_Create(2);
  Peak_AddValue(&queue, 20);
  Peak_AddValue(&queue, 50);
  Peak_AddValue(&queue, 40);
  Peak_AddValue(&queue, 30);
  MaxQueue_Add(&queue1, 15);
  MaxQueue_Add(&queue1, 25);
  CHECK_EQUAL(25, MaxQueue_Max(&queue1));
  CHECK_EQUAL(2, *MaxQueue_Index(&queue1));
  CHECK_EQUAL(15, MaxQueue_Values(&queue1)[0]);
  CHECK_EQUAL(0, Peak_IsDecreasing(&queue));
  Peak_Destroy(&queue);
}

