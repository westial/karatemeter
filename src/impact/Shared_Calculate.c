//
// Created by jaume on 5/14/21.
//
#include <math.h>

double pythagorean_acceleration(int x, int y, int z) {
  return sqrt(pow(sqrt(pow(x, 2) + pow(y, 2)), 2) + pow(z, 2));
}

