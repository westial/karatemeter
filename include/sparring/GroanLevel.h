//
// Created by jaume on 3/21/21.
//

#ifndef KARATESPARRING_INCLUDE_SPARRING_GROANLEVEL_H_
#define KARATESPARRING_INCLUDE_SPARRING_GROANLEVEL_H_

typedef enum {
  VerySoft,
  Soft,
  Medium,
  Hard,
  VeryHard
} GroanLevel;

#define GROANS_LENGTH 5

typedef struct {
  GroanLevel level;
  unsigned int id;
} GroanSoundId;


#endif //KARATESPARRING_INCLUDE_SPARRING_GROANLEVEL_H_
