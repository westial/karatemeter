
#include "CppUTest/TestHarness.h"

extern "C" {
#include "Impact.h"
#include "config/impactconfig.h"
}

// -----------------------------------------------------------------------------

static int expected_result[90];
static unsigned int multi_index;

static void stub_axes_acceleration(int *axes_result) {
  for (int i = 0; i < 3; i++) {
    axes_result[i] = expected_result[multi_index++];
  }
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
    multi_index = 0;
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
  expected_result[1] = 0;
  expected_result[2] = 0;
  Impact_Create(
      (const void *) stub_axes_acceleration,
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
  expected_result[1] = 30;
  expected_result[2] = 30;
  Impact_Create((const void *) stub_axes_acceleration,
                (const void *) stub_countdown_accel_readings_timeout,
                70, 1, PEAK_VALUES_LENGTH);
  Impact_WaitForPeak();
  result = Impact_GetPeak();
  CHECK_EQUAL(0, result);
  Impact_Destroy();
}

TEST(Impact, MeasureImpact) {
  // Python calculation:
  // math.sqrt(30**2+math.sqrt(10**2+20**2)**2) = 37.416573867739416
  int result;
  timeout_after_readings_reached = 1;
  expected_result[0] = 10;
  expected_result[1] = 20;
  expected_result[2] = 30;
  Impact_Create((const void *) stub_axes_acceleration,
                (const void *) stub_countdown_accel_readings_timeout,
                37, 1, PEAK_VALUES_LENGTH);
  Impact_WaitForPeak();
  result = Impact_GetPeak();
  CHECK_EQUAL(37, result);
  Impact_Destroy();
}

TEST(Impact, MaxPeakImpactOnDecrease) {
  int result = 0;
  timeout_after_readings_reached = 1000;
  expected_result[0] = 10;
  expected_result[1] = 10;
  expected_result[2] = 10;
  expected_result[3] = 30;
  expected_result[4] = 30;
  expected_result[5] = 30;
  expected_result[6] = 10;
  expected_result[7] = 10;
  expected_result[8] = 10;
  Impact_Create((const void *) stub_axes_acceleration,
                (const void *) stub_countdown_accel_readings_timeout,
                0, 1, PEAK_VALUES_LENGTH);
  Impact_WaitForPeak();
  result = Impact_GetPeak();
  CHECK_EQUAL(0, spy_timeout);
  CHECK_EQUAL(52, result);
  Impact_Destroy();
}

TEST(Impact, MaxPeakImpactOnDecreaseLongerPeakRange) {
  int result = 0;
  timeout_after_readings_reached = 1000;
  expected_result[0] = 10;
  expected_result[1] = 10;
  expected_result[2] = 10;
  expected_result[3] = 30;
  expected_result[4] = 30;
  expected_result[5] = 30;
  expected_result[6] = 20;
  expected_result[7] = 20;
  expected_result[8] = 20;
  expected_result[9] = 10;
  expected_result[10] = 10;
  expected_result[11] = 10;
  expected_result[12] = 5;
  expected_result[13] = 5;
  expected_result[14] = 5;
  Impact_Create((const void *) stub_axes_acceleration,
                (const void *) stub_countdown_accel_readings_timeout,
                0, 3, PEAK_VALUES_LENGTH);
  if (Impact_WaitForPeak()) {
    result = Impact_GetPeak();
  }
  CHECK_EQUAL(0, spy_timeout);
  CHECK_EQUAL(52, result);
  Impact_Destroy();
}

TEST(Impact, NoImpactDueToLongerPeakRange) {
  int result = 0;
  timeout_after_readings_reached = 3;
  expected_result[0] = 10;
  expected_result[1] = 10;
  expected_result[2] = 10;
  expected_result[3] = 30;
  expected_result[4] = 30;
  expected_result[5] = 30;
  expected_result[6] = 10;
  expected_result[7] = 10;
  expected_result[8] = 10;
  Impact_Create((const void *) stub_axes_acceleration,
                (const void *) stub_countdown_accel_readings_timeout,
                0, 3, PEAK_VALUES_LENGTH);
  Impact_WaitForPeak();
  result = Impact_GetPeak();
  CHECK_EQUAL(1, spy_timeout);
  CHECK_EQUAL(52, result);
  Impact_Destroy();
}

TEST(Impact, MeasureNegativeValue) {
  // Python calculation:
  // math.sqrt(30**2+math.sqrt(10**2+20**2)**2) = 37.416573867739416
  int result;
  timeout_after_readings_reached = 1;
  expected_result[0] = -10;
  expected_result[1] = 20;
  expected_result[2] = -30;
  Impact_Create((const void *) stub_axes_acceleration,
                (const void *) stub_countdown_accel_readings_timeout,
                37, 1, PEAK_VALUES_LENGTH);
  Impact_WaitForPeak();
  result = Impact_GetPeak();
  CHECK_EQUAL(37, result);
  Impact_Destroy();
}