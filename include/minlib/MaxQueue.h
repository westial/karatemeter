//
// Created by jaume on 4/12/21.
//

#ifndef KARATESPARRING_INCLUDE_MINLIB_QUEUE_H_
#define KARATESPARRING_INCLUDE_MINLIB_QUEUE_H_

typedef struct {
  int* values;
  unsigned long index;
  unsigned long length;
  int max;
} MaxQueue;

MaxQueue MaxQueue_Create(unsigned long length);

int MaxQueue_Max(MaxQueue* queue);

void MaxQueue_Add(MaxQueue* queue, int value);

void MaxQueue_Reset(MaxQueue* queue);

int* MaxQueue_Values(MaxQueue* queue);

unsigned long* MaxQueue_Index(MaxQueue* queue);

void MaxQueue_Destroy(MaxQueue* queue);


#endif //KARATESPARRING_INCLUDE_MINLIB_QUEUE_H_
