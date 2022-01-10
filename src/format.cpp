#include "format.h"
#include <string>

using std::string;
using std::to_string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long s) {
  int h = s / 3600;
  s -= h * 3600;
  int m = s / 60;
  s -= m * 60;
  return to_string(h) + ":" + to_string(m) + ":" + to_string(s);
}


