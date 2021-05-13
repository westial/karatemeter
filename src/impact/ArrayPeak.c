//
// Created by jaume on 3/24/21.
//

#include "Peak.h"
#include "MaxQueue.h"

static unsigned long decreasing_distance;

MaxQueue Peak_Create(const unsigned long length, const unsigned long range_distance) {
  MaxQueue queue = MaxQueue_Create(length);
  decreasing_distance = range_distance;
  return queue;
}

void Peak_Destroy(MaxQueue* queue) {
  MaxQueue_Destroy(queue);
}

void Peak_AddValue(MaxQueue* queue, int value) {
  MaxQueue_Add(queue, value);
}

int Peak_IsDecreasing(MaxQueue* queue) {
  unsigned long index = 0;
  unsigned long position;
  unsigned long decreasing = 0;
  while ((position = *MaxQueue_Index(queue) - 1 - index) > 0 && decreasing_distance > index) {
    if (MaxQueue_Values(queue)[position] < MaxQueue_Values(queue)[position - 1]) ++decreasing;
    ++index;
  }
  return decreasing == decreasing_distance && *MaxQueue_Index(queue) >= decreasing_distance;
}

int Peak_GetMaximum(MaxQueue* queue) {
  return MaxQueue_Max(queue);
}

void Peak_Reset(MaxQueue* queue) {
  MaxQueue_Reset(queue);
}