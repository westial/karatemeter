#include "MaxQueue.h"
#include "Display.h"
#include "Impact.h"
#include "MeasureAndDisplayByModeUseCase.h"

static int display_bars;
static int display_with;
static int display_height;
static int session_max;
static MaxQueue display_queue;

void MeasureAndDisplayByModeUseCase_Create(
    const MeasureConfig* measure,
    const DisplayConfig* display) {
  Impact_Create(
      measure->read_acceleration_fn,
      measure->check_timeout_fn,
      measure->sensitive_min,
      measure->sensible_range,
      measure->memory_length
      );
  display_bars = display->bars;
  display_with = display->width;
  display_height = display->height;
  Display_Create(
      display->line_fn,
      display->corner_number_fn,
      display->number_with_caption,
      display_bars,
      display_with,
      display_height);
  display_queue = MaxQueue_Create(display_bars);
  session_max = 0;
}

int MeasureAndDisplayByModeUseCase_Invoke(Mode mode) {
  int impact;
  int peak;
  impact = Impact_WaitForPeak();
  if (0 > impact) {
    return impact;
  }
  peak = Impact_GetPeak();
  MaxQueue_Add(&display_queue, peak);
  if (session_max < MaxQueue_Max(&display_queue)) session_max = MaxQueue_Max(&display_queue);
  switch (mode) {
    case BAR:
      Display_Bars((D_num*) MaxQueue_Values(&display_queue));
      Display_Bar_Max();
      break;
    default:
    case RECORD:Display_Records(peak, session_max);
  }
  return impact;
}

void MeasureAndDisplayByModeUseCase_Destroy() {
  Impact_Destroy();
  Display_Destroy();
  MaxQueue_Destroy(&display_queue);
}