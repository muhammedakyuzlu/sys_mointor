#ifndef PROCESSOR_H
#define PROCESSOR_H
#include<vector>
class Processor {
 public:
  float Utilization();  // DONE: See src/processor.cpp
  // DONE: Declare any necessary private members
 private:
    std::vector<long>  last_measurement{0,0};
};
#endif