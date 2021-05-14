
#include "CppUTest/TestHarness.h"

extern "C" {
#include "Calculate.h"
}

TEST_GROUP(Calculate) {
};

TEST(Calculate, PythagoreanAcceleration) {
  // Python calculation:
  // math.sqrt(30**2+math.sqrt(10**2+20**2)**2) = 37.416573867739416
  DOUBLES_EQUAL(37.4, pythagorean_acceleration(10, 20, 30), 0.1);
  DOUBLES_EQUAL(37.4, pythagorean_acceleration(20, 30, 10), 0.1);
  DOUBLES_EQUAL(37.4, pythagorean_acceleration(30, 20, 10), 0.1);
  DOUBLES_EQUAL(37.4, pythagorean_acceleration(30, -20, 10), 0.1);
  DOUBLES_EQUAL(37.4, pythagorean_acceleration(30, 20, -10), 0.1);
}