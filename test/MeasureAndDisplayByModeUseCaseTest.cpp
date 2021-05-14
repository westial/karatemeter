
#include "CppUTest/TestHarness.h"

extern "C" {
#include "MeasureAndDisplayByModeUseCase.h"
#include "config/impactconfig.h"
#include "config/displayconfig.h"
}

// -----------------------------------------------------------------------------

static int expected_result[90];
static unsigned int expected_index;

static void stub_acceleration(int *axes_result) {
  *axes_result = expected_result[expected_index++];
}

static int timeout_after_readings_reached;
static int spy_timeout;

static int stub_countdown_accel_readings_timeout() {
  spy_timeout = --timeout_after_readings_reached <= 0;
  return spy_timeout;
}

static int spied_display_bar_params[1024];
static int spied_display_bar_params_index;
static void spy_display_line(
    const int start_x, const int start_y, const int end_x, const int end_y) {
  spied_display_bar_params[spied_display_bar_params_index++] = start_x;
  spied_display_bar_params[spied_display_bar_params_index++] = start_y;
  spied_display_bar_params[spied_display_bar_params_index++] = end_x;
  spied_display_bar_params[spied_display_bar_params_index++] = end_y;
}

static int spied_display_number_params[64];
static int spied_display_number_params_index;
static void spy_display_number(
    const int start_x, const int start_y, const int number) {
  spied_display_number_params[spied_display_number_params_index++] = start_x;
  spied_display_number_params[spied_display_number_params_index++] = start_y;
  spied_display_number_params[spied_display_number_params_index++] = number;
}

static D_num spied_display_number_with_caption_params[2];
static void spy_display_number_with_caption(const D_num number, const char caption[], const int position) {
  spied_display_number_with_caption_params[position] = number;
}

static void fake() {};

TEST_GROUP(MeasureAndDisplayByMode) {
  void setup() override {
    expected_index = 0;
    int index;
    spied_display_bar_params_index = 0;
    spied_display_number_params_index = 0;
    for (index = 0;
         index < sizeof(expected_result) / sizeof(expected_result[0]); index++)
      expected_result[index] = -1;
    for (index = 0; index < sizeof(spied_display_bar_params)
        / sizeof(spied_display_bar_params[0]); index++)
      spied_display_bar_params[index] = -1;
    for (index = 0; index < sizeof(spied_display_number_params)
        / sizeof(spied_display_number_params[0]); index++)
      spied_display_number_params[index] = -1;
  }
};

TEST(MeasureAndDisplayByMode, NoImpactNoDisplay) {
  int result;
  int sensitive_min = 0;
  timeout_after_readings_reached = 1;
  expected_result[0] = 0;
  MeasureConfig measure_config = {
      (void *) stub_acceleration,
      (void *) stub_countdown_accel_readings_timeout,
      sensitive_min,
      PEAK_RANGE_DISTANCE,
      PEAK_VALUES_LENGTH
  };
  DisplayConfig display_config = {
      (void *) spy_display_line,
      (void *) spy_display_number,
      (void *) fake,
      DISPLAY_WIDTH,
      DISPLAY_WIDTH,
      DISPLAY_HEIGHT
  };
  MeasureAndDisplayByModeUseCase_Create(&measure_config, &display_config);
  result = MeasureAndDisplayByModeUseCase_Invoke(BAR);
  CHECK_EQUAL(0, result);
  CHECK_EQUAL(1, spied_display_number_params[0]);
  CHECK_EQUAL(1, spied_display_number_params[1]);
  CHECK_EQUAL(0, spied_display_number_params[2]);
  MeasureAndDisplayByModeUseCase_Destroy();
}

TEST(MeasureAndDisplayByMode, OneImpact) {
  int result;
  int sensitive_min = 18;
  timeout_after_readings_reached = 1;
  expected_result[0] = 19;
  MeasureConfig measure_config = {
      (void *) stub_acceleration,
      (void *) stub_countdown_accel_readings_timeout,
      sensitive_min,
      PEAK_RANGE_DISTANCE,
      PEAK_VALUES_LENGTH
  };
  DisplayConfig display_config = {
      (void *) spy_display_line,
      (void *) spy_display_number,
      (void *) fake,
      DISPLAY_WIDTH,
      DISPLAY_WIDTH,
      DISPLAY_HEIGHT
  };
  MeasureAndDisplayByModeUseCase_Create(&measure_config, &display_config);
  result = MeasureAndDisplayByModeUseCase_Invoke(BAR);
  CHECK_EQUAL(0, result);
  CHECK_EQUAL(0, spied_display_bar_params[0]);
  CHECK_EQUAL(0, spied_display_bar_params[1]);
  CHECK_EQUAL(0, spied_display_bar_params[2]);
  CHECK_EQUAL(19, spied_display_bar_params[3]);
  MeasureAndDisplayByModeUseCase_Destroy();
}

TEST(MeasureAndDisplayByMode, MaxRemainsAllSession) {
  timeout_after_readings_reached = 1;
  MeasureConfig measure_config = {
      (void *) stub_acceleration,
      (void *) stub_countdown_accel_readings_timeout,
      1,
      1,
      2     // After 2 peaks the max queue must reset but the
                          // maximum value in the use case remains.
  };
  DisplayConfig display_config = {
      (void *) spy_display_line,
      (void *) spy_display_number,
      (void *) spy_display_number_with_caption,
      DISPLAY_WIDTH,
      DISPLAY_WIDTH,
      DISPLAY_HEIGHT
  };
  MeasureAndDisplayByModeUseCase_Create(&measure_config, &display_config);
  expected_result[0] = 52;
  MeasureAndDisplayByModeUseCase_Invoke(RECORD);
  expected_result[1] = 37;
  MeasureAndDisplayByModeUseCase_Invoke(RECORD);
  expected_result[2] = 37;
  MeasureAndDisplayByModeUseCase_Invoke(RECORD);
  expected_result[3] = 37;
  MeasureAndDisplayByModeUseCase_Invoke(RECORD);
  CHECK_EQUAL(37, spied_display_number_with_caption_params[0]);
  CHECK_EQUAL(52, spied_display_number_with_caption_params[1]);
  MeasureAndDisplayByModeUseCase_Destroy();
}
