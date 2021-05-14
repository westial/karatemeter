//
// Created by jaume on 5/14/21.
//

#include "Impact.h"
#include "Peak.h"
#include <MaxQueue.h>

static void (*read_acceleration) (int* axis_result) = 0;

static int maximum_value;
static int acceleration[1];
static int sensitive_min_value;
static MaxQueue peak_queue;

/**
 * Wait for timeout.
 * @return 1 if timeout is reached, 0 when timeout is not reached and -1 on error.
 */
static int (*break_on_timeout) () = 0;

void Impact_Create(
    const void* read_acceleration_fn,
    const void* break_on_timeout_fn,
    const int sensitive_min,
    const unsigned long peak_range,
    unsigned long peak_values_length) {
  read_acceleration = read_acceleration_fn;
  sensitive_min_value = sensitive_min;
  break_on_timeout = break_on_timeout_fn;
  peak_queue = Peak_Create(peak_values_length, peak_range);
}

void Impact_Destroy() {
  Peak_Destroy(&peak_queue);
}

int Impact_WaitForPeak() {
  maximum_value = 0;
  do {
    read_acceleration(acceleration);
    if (sensitive_min_value <= *acceleration) {
      Peak_AddValue(&peak_queue, *acceleration);
      maximum_value = Peak_GetMaximum(&peak_queue);
    }
  } while (0 == Peak_IsDecreasing(&peak_queue) && !break_on_timeout());
  if (-1 == maximum_value) return -1;
  return break_on_timeout() ? 0 : 1;
}

int Impact_GetPeak() {
  Peak_Reset(&peak_queue);
  return maximum_value;
}