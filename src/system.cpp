#include "system.h"
#include "format.h"
#include <unistd.h>

#include <array>
#include <cstddef>
#include <set>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include <iostream>
using namespace std;

/*You need to complete the mentioned TODOs in order to satisfy the rubric
criteria "The student will be able to extract and display basic data about the
system."

You need to properly format the uptime. Refer to the comments mentioned in
format. cpp for formatting the uptime.*/

// DONE: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  processes_={};

  vector<int> pids = LinuxParser::Pids();
  for (int i=0;(unsigned)i<pids.size();i++) {
    Process tem;
    tem._pid = pids[i];
    tem._user = LinuxParser::User(pids[i]);
    tem._command =LinuxParser::Command(pids[i]);
    tem._cpuUtilization = LinuxParser::CpuUtilization(pids[i]);
    tem._ram = LinuxParser::Ram(pids[i]);
    tem._upTime = LinuxParser::UpTime(pids[i]);
    processes_.push_back(tem);
  }

  sort(processes_.begin(), processes_.end());
  return processes_;
}

// DONE Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// DONE: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// DONE: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// DONE: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// DONE: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// DONE: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
