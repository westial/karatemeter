//
// Created by jaume on 3/21/21.
//

#include "Impact.h"
#include "Peak.h"
#include <math.h>
#include <MaxQueue.h>

static void (*read_acceleration) (int* axes_result) = 0;

static int maximum_value;
static double acceleration;
static int sensitive_min_value;
static MaxQueue peak_queue;

/**
 * Wait for timeout.
 * @return 1 if timeout is reached, 0 when timeout is not reached and -1 on error.
 */
static int (*break_on_timeout) () = 0;

void Impact_Create(
    const void* read_x3_acceleration_fn,
    const void* break_on_timeout_fn,
    const int sensitive_min,
    const unsigned long peak_range,
    unsigned long peak_values_length) {
  read_acceleration = read_x3_acceleration_fn;
  sensitive_min_value = sensitive_min;
  break_on_timeout = break_on_timeout_fn;
  peak_queue = Peak_Create(peak_values_length, peak_range);
}

void Impact_Destroy() {
  Peak_Destroy(&peak_queue);
}

/**
 * Calculate the acceleration of the three axes based on the pythagorean theorem.
 * The rectangle created from the point 0 to the point where the x, y and z
 * converge is what I call "Face". The diagonal of the Face is the value that I
 * am looking for.
 *
 * Face width = z.
 * Face height = Square root of x^2 plus y^2.
 * Diagonal = Square root of (Face width)^2 plus (Face height)^2.
 *
 * @param x
 * @param y
 * @param z
 * @return
 */
double pythagorean_acceleration(int x, int y, int z) {
  return sqrt(pow(sqrt(pow(x, 2) + pow(y, 2)), 2) + pow(z, 2));
}

int Impact_WaitForPeak() {
  int xyz[3];
  maximum_value = 0;
  do {
    read_acceleration(xyz);
    acceleration = pythagorean_acceleration(xyz[0], xyz[1], xyz[2]);
    if (sensitive_min_value <= acceleration) {
      Peak_AddValue(&peak_queue, (int) round(acceleration));
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