#include "linux_parser.h"
#include"system.h"
#include <dirent.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "processor.h"

using std::cout;
using std::endl;
using std::stof;
using std::string;
using std::to_string;
using std::vector;


// return the value of given key on the specific file
template <typename T>
T func(string path, string word);
// return func<int>(kProcDirectory+kStatFilename,"procs_running");

template <typename T>
T func2(string path);

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float MemTotal =
      func<float>(kProcDirectory + kMeminfoFilename, filterMemTotalString);
  float MemAvailable =
      func<float>(kProcDirectory + kMeminfoFilename, filterMemAvailable);
  // Memory utilization = used memory / total memory
  return (MemTotal - MemAvailable) / MemTotal;
}

// DONE: Read and return the system uptime long
long LinuxParser::UpTime() {
  return func2<long>(kProcDirectory + kUptimeFilename);
}

// DONE: Read and return the number of jiffies for the system
vector<long> LinuxParser::Jiffies() {
  int user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  string line, _;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> _ >> user >> nice >> system >> idle >> iowait >> irq >>
        softirq >> steal >> guest >> guest_nice;
  }
  // total_jiffies 'ALL' , work_jiffies'user,nice,system'
  // presentage work/total
  return {user + nice + system + idle + iowait + irq + softirq + steal + guest +
              guest_nice,
          user + nice + system};
}

// TODO: NEED Read and return CPU utilization
float LinuxParser::CpuUtilization(int pid) {
  string line, key, _s;
  int utime, stime, cutime, cstime, starttime, uptime, _i;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);  // 22
    linestream >> _i >> _s >> _s >> _i >> _i >> _i >> _i >> _i >> _i >> _i >>
        _i >> _i >> _i >> utime >> stime >> cutime >> cstime >> _i >> _i >>
        _i >> _i >> starttime >> _i;
  }

  std::ifstream filestream2(kProcDirectory + kUptimeFilename);
  if (filestream2.is_open()) {
    std::getline(filestream2, line);
    std::istringstream linestream(line);
    linestream >> uptime >> _i;
  }
  float total_time = utime + stime + cutime + cstime;
  float seconds = uptime - (starttime / sysconf(_SC_CLK_TCK));
  float cpu_usage = ((total_time / sysconf(_SC_CLK_TCK)) / seconds);
  return cpu_usage;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  return func<int>(kProcDirectory + kStatFilename, filterProcesses);
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  return func<int>(kProcDirectory + kStatFilename, filterRunningProcesses);
}

// TODO: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string value, _;
  string line =
      func2<string>(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  std::istringstream linestream(line);
  linestream >> value >> _;
  return value;
}

// TODO: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  return to_string(
      func<int>(kProcDirectory + std::to_string(pid) + kStatusFilename,
                filterProcMem) /
      1024);
}

// TODO: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  // /proc/[PID]/status
  return func<string>(kProcDirectory + std::to_string(pid) + kStatusFilename,
                      filterUID);
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string line, key, value, _;
  string _uid = Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> value >> _ >> key) {
        if (key == _uid) {
          return value;
        }
      }
    }
  }
  return string();
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  long starttime, _i;
  string line, _s;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);  // 22
    linestream >> _i >> _s >> _s >> _i >> _i >> _i >> _i >> _i >> _i >> _i >>
        _i >> _i >> _i >> _i >> _i >> _i >> _i >> _i >> _i >> _i >> _i >>
        starttime >> _i;
  }
  
// You need to extract the 22nd variable in this file and then convert this jiffy value into seconds and since 
// this 22nd value denotes the time of the program since the boot time - 
// you will need to subtract this from system uptime to get the process uptime!
  return System().UpTime()-starttime/sysconf(_SC_CLK_TCK);
}

template <typename T>
T func(string path, string word) {
  string line, key;
  T value;
  std::ifstream filestream(path);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == word) return value;
      }
    }
  }
  return value;
}

template <typename T>
T func2(string path) {
  string line;
  T time;
  std::ifstream stream(path);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> time;
  }
  return time;
}
