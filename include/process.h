#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int& Pid();                              // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp
  friend class System;
  // TODO: Declare any necessary private members
 private:
  int _pid=0;
  std::string _user=std::string();
  std::string _command=std::string();
  float _cpuUtilization=0;
  std::string _ram=std::string();
  long int _upTime=0;
};

#endif