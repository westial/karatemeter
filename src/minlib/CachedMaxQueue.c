//
// Created by jaume on 4/12/21.
//

#include <stdlib.h>
#include "MaxQueue.h"

static void update_max(MaxQueue *queue, int value);
static void enqueue_value(MaxQueue *queue, int value);
static void init_values(MaxQueue *queue);

MaxQueue MaxQueue_Create(const unsigned long length) {
  MaxQueue queue;
  queue.length = length;
  queue.values = (int*) calloc(length, sizeof(int));
  MaxQueue_Reset(&queue);
  return queue;
}

static void init_values(MaxQueue *queue) {
  unsigned long index;
  for (index = 0; index < queue->length; index++) {
    queue->values[index] = 0;
  }
}

void MaxQueue_Reset(MaxQueue *queue) {
  init_values(queue);
  queue->index = 0;
  queue->max = -1;
}

int *MaxQueue_Values(MaxQueue *queue) {
  return queue->values;
}

unsigned long *MaxQueue_Index(MaxQueue *queue) {
  return &(queue->index);
}

static void update_max(MaxQueue *queue, const int value) {
  const int absolute = abs(value);
  if (queue->max < absolute) queue->max = absolute;
}

static void enqueue_value(MaxQueue *queue, const int value) {
  if (queue->length > queue->index) queue->values[queue->index++] = value;
  else {
    unsigned long index = 0;
    while (index < queue->length - 1) {
      queue->values[index] = queue->values[1 + index];
      index++;
    }
    update_max(queue, queue->values[index]);
    queue->values[index] = value;
  }
}

int MaxQueue_Max(MaxQueue *queue) {
  return queue->max;
}

void MaxQueue_Add(MaxQueue *queue, int value) {
  enqueue_value(queue, value);
  update_max(queue, value);
}

void MaxQueue_Destroy(MaxQueue *queue) {
  free(queue->values);
}