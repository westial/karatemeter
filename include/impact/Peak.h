//
// Created by jaume on 3/24/21.
//

#ifndef KARATESPARRING_INCLUDE_IMPACT_PEAK_H_
#define KARATESPARRING_INCLUDE_IMPACT_PEAK_H_

#include <MaxQueue.h>

MaxQueue Peak_Create(unsigned long values_len, unsigned long range_distance);

void Peak_Destroy(MaxQueue* queue);

void Peak_AddValue(MaxQueue* queue, int value);

int Peak_IsDecreasing(MaxQueue* queue);

int Peak_GetMaximum(MaxQueue* queue);

void Peak_Reset(MaxQueue* queue);

#endif //KARATESPARRING_INCLUDE_IMPACT_PEAK_H_
