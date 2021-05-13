//
// Created by jaume on 3/21/21.
//

#ifndef KARATESPARRING_SRC_SPARRING_SPARRING_H_
#define KARATESPARRING_SRC_SPARRING_SPARRING_H_

#include "GroanLevel.h"

void Sparring_Create(const void* driver, GroanSoundId groan_conf[], unsigned int groans_len);

int Sparring_Groan(GroanLevel level);

#endif //KARATESPARRING_SRC_SPARRING_SPARRING_H_
