
#include "CppUTest/TestHarness.h"

extern "C" {
#include "Impact.h"
#include "config/impactconfig.h"
}

// -----------------------------------------------------------------------------

static int expected_result[90];
static unsigned int expected_index;

static void stub_acceleration(int *axis_result) {
  *axis_result = expected_result[expected_index++];
}

static int timeout_after_readings_reached;
static int spy_timeout;

static int stub_countdown_accel_readings_timeout() {
  spy_timeout = --timeout_after_readings_reached <= 0;
  return spy_timeout;
}

static int fake_never_timeout() {
  return 0;
}

static void fake() {};

TEST_GROUP(Impact) {
  void setup() override {
    expected_index = 0;
    int index;
    for (index = 0;
         index < sizeof(expected_result) / sizeof(expected_result[0]); index++)
      expected_result[index] = -1;
  }
};

TEST(Impact, NoImpact) {
  int result ;
  int timed_out;
  timeout_after_readings_reached = 1;
  expected_result[0] = 0;
  Impact_Create(
      (const void *) stub_acceleration,
      (const void *) stub_countdown_accel_readings_timeout,
      0, 1, PEAK_VALUES_LENGTH);
  timed_out = 0 == Impact_WaitForPeak();
  result = Impact_GetPeak();
  CHECK_TRUE(timed_out);
  CHECK_EQUAL(0, result);
  Impact_Destroy();
}

TEST(Impact, IgnoredLowerImpact) {
  int result;
  timeout_after_readings_reached = 1;
  expected_result[0] = 30;
  Impact_Create((const void *) stub_acceleration,
                (const void *) stub_countdown_accel_readings_timeout,
                70, 1, PEAK_VALUES_LENGTH);
  Impact_WaitForPeak();
  result = Impact_GetPeak();
  CHECK_EQUAL(0, result);
  Impact_Destroy();
}

TEST(Impact, MaxPeakImpactOnDecrease) {
  int result = 0;
  timeout_after_readings_reached = 1000;
  expected_result[0] = 10;
  expected_result[1] = 30;
  expected_result[2] = 10;
  Impact_Create((const void *) stub_acceleration,
                (const void *) stub_countdown_accel_readings_timeout,
                0, 1, PEAK_VALUES_LENGTH);
  Impact_WaitForPeak();
  result = Impact_GetPeak();
  CHECK_EQUAL(0, spy_timeout);
  CHECK_EQUAL(30, result);
  Impact_Destroy();
}

TEST(Impact, MaxPeakImpactOnDecreaseLongerPeakRange) {
  int result = 0;
  timeout_after_readings_reached = 1000;
  expected_result[0] = 10;
  expected_result[1] = 30;
  expected_result[2] = 20;
  expected_result[3] = 10;
  expected_result[4] = 5;
  Impact_Create((const void *) stub_acceleration,
                (const void *) stub_countdown_accel_readings_timeout,
                0, 3, PEAK_VALUES_LENGTH);
  if (Impact_WaitForPeak()) {
    result = Impact_GetPeak();
  }
  CHECK_EQUAL(0, spy_timeout);
  CHECK_EQUAL(30, result);
  Impact_Destroy();
}

TEST(Impact, NoImpactDueToLongerPeakRange) {
  int result;
  timeout_after_readings_reached = 3;
  expected_result[0] = 10;
  expected_result[1] = 30;
  expected_result[2] = 10;
  Impact_Create((const void *) stub_acceleration,
                (const void *) stub_countdown_accel_readings_timeout,
                0, 3, PEAK_VALUES_LENGTH);
  Impact_WaitForPeak();
  result = Impact_GetPeak();
  CHECK_EQUAL(1, spy_timeout);
  CHECK_EQUAL(30, result);
  Impact_Destroy();
}