#include "processor.h"

#include <unistd.h>
#include <vector>

#include "linux_parser.h"

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
  // total_jiffies 'ALL' , work_jiffies'user,nice,system'  --> LinuxParser::Jiffies()
  // presentage work/total  
  std::vector<long> tem{last_measurement[0], last_measurement[1]};
  last_measurement = LinuxParser::Jiffies();
  // calculate the diffrent between two measurement
  float t = last_measurement.front() / 1.f - tem.front() / 1.f;
  float w = last_measurement.back() / 1.f - tem.back() / 1.f;
  return w / t;
}