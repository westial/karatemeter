//
// Created by jaume on 4/12/21.
//

#ifndef KARATESPARRING_INCLUDE_USECASE_MEASUREANDDISPLAYBYMODEUSECASE_H_
#define KARATESPARRING_INCLUDE_USECASE_MEASUREANDDISPLAYBYMODEUSECASE_H_

#include "config/typeconfig.h"

enum DISPLAY_MODE {
  BAR,
  RECORD
} typedef Mode;

typedef struct {
  void* read_acceleration_fn;
  void* check_timeout_fn;
  int sensitive_min;
  unsigned long sensible_range;
  unsigned long memory_length;
} MeasureConfig;

typedef struct {
  void* line_fn;
  void* corner_number_fn;
  void* number_with_caption;
  D_num bars;
  D_num width;
  D_num height;
} DisplayConfig;

void MeasureAndDisplayByModeUseCase_Create(
    const MeasureConfig* measure,
    const DisplayConfig* display);

/**
 * Listen for an impact and display results or do nothing in case of error
 * or timeout.
 * @return int -1 on error, 0 on timeout, 1 on success.
 */
int MeasureAndDisplayByModeUseCase_Invoke(Mode mode);

void MeasureAndDisplayByModeUseCase_Destroy();

#endif //KARATESPARRING_INCLUDE_USECASE_MEASUREANDDISPLAYBYMODEUSECASE_H_
