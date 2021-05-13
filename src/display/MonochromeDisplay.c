#include "Display.h"

#define POSITION_CORNER_NUM_X         1
#define POSITION_CORNER_NUM_Y         1

static void (*display_line)(
    const D_num start_x,
    const D_num start_y,
    const D_num end_x,
    const D_num end_y
) = 0;

static void (*display_number)(
    const D_num start_x,
    const D_num start_y,
    const D_num number) = 0;

static void (*display_number_with_caption)(
    const D_num number,
    const char caption[],
    const int position) = 0;

static D_num display_bars;
static D_num display_height;
static float height_factor;
static D_num max_height_value;

static void set_factor(const D_num heights[]);
static float scale_height(float value);

/**
 * Convert an integer to a string and return the string length
 * @param number input integer
 * @param result string
 * @param max_length maximum length of the output string
 * @return string length
 */

void Display_Create(
    const void *display_line_fn,
    const void *display_corner_number_fn,
    const void *display_number_with_caption_fn,
    D_num bars,
    D_num width,
    D_num height) {
  display_line = display_line_fn;
  display_number = display_corner_number_fn;
  display_number_with_caption = display_number_with_caption_fn;
  display_bars = bars;
  display_height = height;
}

static void set_factor(const D_num heights[]) {
  D_num i;
  max_height_value = 0;
  for (i = 0; i < display_bars; i++)
    if (heights[i] > max_height_value) max_height_value = heights[i];
  if (display_height < max_height_value)
    height_factor = (float) max_height_value / (float) display_height;
  else
    height_factor = 1;
}

static float scale_height(float value) {
  return value / height_factor;
}

void Display_Bars(D_num heights[]) {
  D_num x;
  set_factor(heights);
  for (x = 0; x < display_bars; x++) {
    display_line(x, 0, x, (D_num) scale_height((float) heights[x]));
  }
}

void Display_Bar_Max() {
  display_number(POSITION_CORNER_NUM_X, POSITION_CORNER_NUM_Y, max_height_value);
}

void Display_Records(D_num last, D_num session_max) {
  display_number_with_caption(last, "last", 0);
  display_number_with_caption(session_max, "record", 1);
}

void Display_Destroy() {}