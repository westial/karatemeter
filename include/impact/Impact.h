//
// Created by jaume on 3/21/21.
//

#ifndef KARATESPARRING_INCLUDE_IMPACT_IMPACT_H_
#define KARATESPARRING_INCLUDE_IMPACT_IMPACT_H_

void Impact_Create(
    const void* read_x3_acceleration_fn,
    const void* break_on_timeout_fn,
    int sensitive_min,
    unsigned long peak_range,
    unsigned long peak_values_length);

/**
 * Listen to a hit peak.
 * @return int 1 on getting a peak, 0 on timeout and -1 on error.
 */
int Impact_WaitForPeak();

int Impact_GetPeak();

void Impact_Destroy();

#endif //KARATESPARRING_INCLUDE_IMPACT_IMPACT_H_
