#include "CppUTest/TestHarness.h"

extern "C" {
#include "Sparring.h"
#include "GroanLevel.h"
}

// -----------------------------------------------------------------------------

int witness_result = -1;
unsigned int witness_sound_id;

static unsigned int fake_play_sound(const unsigned int sound_id) {
  witness_result = 1;
  witness_sound_id = sound_id;
  return 1;
}

TEST_GROUP(Sparring) {
};

TEST(Sparring, Groan) {
  GroanSoundId groans[] = {{VerySoft, 1}};
  Sparring_Create((const void *) fake_play_sound, groans, GROANS_LENGTH);
  Sparring_Groan(VerySoft);
  CHECK_EQUAL(1, witness_result);
}

TEST(Sparring, HardGroan) {
  GroanSoundId groans[5];
  groans[0] = {VerySoft, 1};
  groans[1] = {Hard, 2};
  Sparring_Create((const void *) fake_play_sound, groans, GROANS_LENGTH);
  Sparring_Groan(Hard);
  CHECK_EQUAL(2, witness_sound_id);
}