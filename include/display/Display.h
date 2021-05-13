//
// Created by jaume on 4/11/21.
//

#ifndef KARATESPARRING_INCLUDE_DISPLAY_DISPLAY_H_
#define KARATESPARRING_INCLUDE_DISPLAY_DISPLAY_H_

#include "config/typeconfig.h"

void Display_Create(
    const void *display_line_fn,
    const void *display_corner_number_fn,
    const void *display_number_with_caption_fn,
    D_num bars,
    D_num width,
    D_num height);

void Display_Bars(D_num heights[]);

void Display_Bar_Max();

void Display_Records(D_num last, D_num session_max);

void Display_Destroy();

#endif //KARATESPARRING_INCLUDE_DISPLAY_DISPLAY_H_
