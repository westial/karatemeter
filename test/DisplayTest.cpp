#include "CppUTest/TestHarness.h"

extern "C" {
#include "Display.h"
}

// -----------------------------------------------------------------------------

static D_num bars;
static D_num width;
static D_num height;
static D_num text_length;

static D_num spied_display_bar_params[255];
static D_num spied_display_bar_params_index;
static void spy_display_line(
    const D_num start_x, const D_num start_y, const D_num end_x, const D_num end_y) {
  spied_display_bar_params[spied_display_bar_params_index++] = start_x;
  spied_display_bar_params[spied_display_bar_params_index++] = start_y;
  spied_display_bar_params[spied_display_bar_params_index++] = end_x;
  spied_display_bar_params[spied_display_bar_params_index++] = end_y;
}

static D_num spied_display_number_params[64];
static D_num spied_display_number_params_index;
static void spy_display_number(
    const D_num start_x, const D_num start_y, const D_num number) {
  spied_display_number_params[spied_display_number_params_index++] = start_x;
  spied_display_number_params[spied_display_number_params_index++] = start_y;
  spied_display_number_params[spied_display_number_params_index++] = number;
}

static D_num spied_display_number_with_caption_params[2];
static char spied_display_caption_param[2][16];
static void spy_display_number_with_caption(const D_num number, const char caption[], const int position) {
  spied_display_number_with_caption_params[position] = number;
  int c;
  for (c = 0; c < 16; c ++)
    spied_display_caption_param[position][c] = caption[c];
}

static void fake() {};

TEST_GROUP(Display) {
  void setup() override {
    bars = 32;
    width = 128;
    height = 32;
    spied_display_bar_params_index = 0;
    spied_display_number_params_index = 0;
    int index;
    int index_text;
    for (index = 0; index < (sizeof(spied_display_caption_param)
        / sizeof(spied_display_caption_param[0])); index++) {
      for (index_text = 0; index_text < (sizeof(spied_display_caption_param[0])
          / sizeof(spied_display_caption_param[0][0])); index_text++) {
        spied_display_caption_param[index][index_text] = '\0';
      }
    }
    for (index = 0; index < (sizeof(spied_display_number_with_caption_params)
        / sizeof(spied_display_number_with_caption_params[0])); index++)
      spied_display_number_with_caption_params[index] = 0;
    for (index = 0; index < (sizeof(spied_display_bar_params)
        / sizeof(spied_display_bar_params[0])); index++)
      spied_display_bar_params[index] = -1;
    for (index = 0; index < (sizeof(spied_display_number_params)
        / sizeof(spied_display_number_params[0])); index++)
      spied_display_number_params[index] = -1;
  }
};

TEST(Display, DisplayABar) {
  bars = 1;
  width = 128;
  D_num bars_values[] = {10};
  Display_Create(
      (const void *) spy_display_line,
      (const void *) fake,
      (const void *) fake,
      bars,
      width,
      height);
  Display_Bars(bars_values);
  CHECK_EQUAL(0, spied_display_bar_params[0]);
  CHECK_EQUAL(0, spied_display_bar_params[1]);
  CHECK_EQUAL(0, spied_display_bar_params[2]);
  CHECK_EQUAL(10, spied_display_bar_params[3]);
}

TEST(Display, DisplayTwoBars) {
  bars = 2;
  width = 128;
  D_num bars_values[] = {10, 15};
  Display_Create(
      (const void *) spy_display_line,
      (const void *) fake,
      (const void *) fake,
      bars,
      width,
      height);
  Display_Bars(bars_values);
  CHECK_EQUAL(1, spied_display_bar_params[4]);
  CHECK_EQUAL(0, spied_display_bar_params[5]);
  CHECK_EQUAL(1, spied_display_bar_params[6]);
  CHECK_EQUAL(15, spied_display_bar_params[7]);
}

TEST(Display, DisplayAnOverHeightedBar) {
  bars = 1;
  width = 128;
  height = 32;
  D_num bars_values[] = {(D_num)(height * 2)};
  Display_Create(
      (const void *) spy_display_line,
      (const void *) fake,
      (const void *) fake,
      bars,
      width,
      height);
  Display_Bars(bars_values);
  CHECK_EQUAL(0, spied_display_bar_params[0]);
  CHECK_EQUAL(0, spied_display_bar_params[1]);
  CHECK_EQUAL(0, spied_display_bar_params[2]);
  CHECK_EQUAL(32, spied_display_bar_params[3]);
}

TEST(Display, DisplayThreeOverHeightedBars) {
  bars = 3;
  width = 128;
  height = 32;
  D_num bars_values[] = {(D_num)(height * 2), (D_num)(height * 4), (D_num)(height * 3)};
  Display_Create(
      (const void *) spy_display_line,
      (const void *) fake,
      (const void *) fake,
      bars,
      width,
      height);
  Display_Bars(bars_values);
  CHECK_EQUAL(0, spied_display_bar_params[0]);
  CHECK_EQUAL(0, spied_display_bar_params[1]);
  CHECK_EQUAL(0, spied_display_bar_params[2]);
  CHECK_EQUAL(16, spied_display_bar_params[3]);
  CHECK_EQUAL(1, spied_display_bar_params[4]);
  CHECK_EQUAL(0, spied_display_bar_params[5]);
  CHECK_EQUAL(1, spied_display_bar_params[6]);
  CHECK_EQUAL(32, spied_display_bar_params[7]);
  CHECK_EQUAL(2, spied_display_bar_params[8]);
  CHECK_EQUAL(0, spied_display_bar_params[9]);
  CHECK_EQUAL(2, spied_display_bar_params[10]);
  CHECK_EQUAL(24, spied_display_bar_params[11]);
}

TEST(Display, DisplayTheMaximumHeight) {
  bars = 5;
  width = 128;
  D_num bars_values[] = {1, 2, 7, 1, 6};
  Display_Create(
      (const void *) spy_display_line,
      (const void *) spy_display_number,
      (const void *) fake,
      bars,
      width,
      height);
  Display_Bars(bars_values);
  Display_Bar_Max();
  CHECK_EQUAL(1, spied_display_number_params[0]);
  CHECK_EQUAL(1, spied_display_number_params[1]);
  CHECK_EQUAL(7, spied_display_number_params[2]);
}

TEST(Display, DisplayNoHeight) {
  bars = 1;
  width = 128;
  D_num bars_values[] = {0};
  Display_Create(
      (const void *) spy_display_line,
      (const void *) spy_display_number,
      (const void *) fake,
      bars,
      width,
      height);
  Display_Bars(bars_values);
  Display_Bar_Max();
  CHECK_EQUAL(1, spied_display_number_params[0]);
  CHECK_EQUAL(1, spied_display_number_params[1]);
  CHECK_EQUAL(0, spied_display_number_params[2]);
}

TEST(Display, DisplayRecordsSeparately) {
  Display_Create(
      (const void *) spy_display_line,
      (const void *) spy_display_number,
      (const void *) spy_display_number_with_caption,
      bars,
      width,
      height);
  Display_Records(200, 250);
  CHECK_EQUAL(200, spied_display_number_with_caption_params[0]);
  STRCMP_EQUAL("last", spied_display_caption_param[0]);
  CHECK_EQUAL(250, spied_display_number_with_caption_params[1]);
  STRCMP_EQUAL("record", spied_display_caption_param[1]);
}
