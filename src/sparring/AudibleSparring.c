//
// Created by jaume on 3/21/21.
//

#include "Sparring.h"

/**
 * Play the sound according the given Id.
 * @param unsigned int Sound Id
 * @return int 1 on success or -1 on error.
 */
static const int (*play_sound)(const unsigned int sound_id) = 0;
static const GroanSoundId* groans;
static unsigned int groans_length;

void Sparring_Create(const void* driver, GroanSoundId groan_conf[], const unsigned int groans_len) {
  play_sound = driver;
  groans = groan_conf;
  groans_length = groans_len;
}

int Sparring_Groan(const GroanLevel level) {
  unsigned int conf_index;
  for (conf_index = 0; conf_index <= groans_length; conf_index ++) {
    if (level == groans[conf_index].level)
      return play_sound(groans[conf_index].id);
  }
  return -1;
}

