#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "processor.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::cout;
using std::endl;
using std::to_string;
// return the value of given key on the specific file
template <typename T>
T func(string path, string word);
// return func<int>(kProcDirectory+kStatFilename,"procs_running");

// run command and get the output
std::string exec(const char* cmd);

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
  float MemTotal = func<float>(kProcDirectory + kMeminfoFilename, "MemTotal:");
  float MemAvailable =
      func<float>(kProcDirectory + kMeminfoFilename, "MemAvailable:");
  // Memory utilization = used memory / total memory
  return (MemTotal - MemAvailable) / MemTotal;
}

// DONE: Read and return the system uptime long
long LinuxParser::UpTime() {
  return func2<long>(kProcDirectory + kUptimeFilename);

  // string line;
  // long time,_;
  // std::ifstream stream(kProcDirectory + kUptimeFilename);
  // if (stream.is_open()) {
  //   std::getline(stream, line);
  //   std::istringstream linestream(line);
  //   linestream >> time >>_;
  // }
  // return time;
}

// DONE: Read and return the number of jiffies for the system
vector<long> LinuxParser::Jiffies() { 
  int user,nice,system,idle,iowait,irq,softirq,steal,guest,guest_nice;
  string line,_;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >>_ >>user>>nice>>system>>idle>>iowait>>irq>>softirq>>steal>>guest>>guest_nice;  
  }
  // total_jiffies 'ALL' , work_jiffies'user,nice,system' 
  // presentage work/total
  return {user+nice+system+idle+iowait+irq+softirq+steal+guest+guest_nice,   user+nice+system};
  }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO???: NEED Read and return CPU utilization
float LinuxParser::CpuUtilization(int pid) {

  
  string line, key,_s;
  int utime,stime,cutime,cstime,starttime,uptime,_i;
  // 1 (systemd) S 0 1 1 0 -1 4194560 18639 247103 155 7331 16 73 298 765 20 0 1 0 9 171732992 2878 
  // 18446744073709551615 1 1 0 0 0 0 671173123 4096 1260 0 0 0 17 2 0 0 9 0 0 0 0 0 0 0 0 0 
  std::ifstream filestream(kProcDirectory+ std::to_string(pid)+kStatFilename);
  if (filestream.is_open()) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      linestream >>_i>>_s >>_s >>_i>>_i>>_i>>_i>>_i>>_i>>_i>>_i>>_i>>_i>>utime>>stime>>cutime>>cstime>>_i>>_i>>_i>>_i>>starttime>>_i ;
      //cout <<"****"<< to_string(pid) <<" **" << to_string(utime)<<" "<<to_string(stime)<<" "<<to_string(cutime)<<" "<<to_string(cstime)<<" "<<to_string(starttime)<<"\n\n\n "<<endl;

  }
  std::ifstream filestream2(kProcDirectory+kUptimeFilename);
  if (filestream2.is_open()) {
      std::getline(filestream2, line);
      std::istringstream linestream(line);
      linestream >>uptime>> _i;

  }

float total_time = utime + stime + cutime + cstime;
float seconds = uptime - (starttime / sysconf(_SC_CLK_TCK));
float cpu_usage = 100 * ((total_time / sysconf(_SC_CLK_TCK)) / seconds);


// float cpu_totla =  Processor().Utilization();
// (cpu_totla * 100) / (cpu_totla + cpu_usage)


// cout <<"///\n\n"<< total_time <<" **" << seconds << "** "<<  cpu_usage << endl;
  
  return cpu_usage; 
  
  } 

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  return func<int>(kProcDirectory + kStatFilename, "processes");
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  return func<int>(kProcDirectory + kStatFilename, "procs_running");
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { return string(); }

// TODO: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  
  // /proc/[PID]/status
   string a =  func<string>(kProcDirectory+ std::to_string(pid)+kStatusFilename,"Uid:");
  //std::cout <<" "<< a;
  //std::cout << "\n\n\n\n" <<std::to_string(pid)<<"\n\n"<<a<<std::endl;   
  
  return a; }

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string line, key, value,_;
  string _uid=Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {

      std::replace(line.begin(), line.end(), ':', ' ');  
      std::istringstream linestream(line);     
      while (linestream >>value >> _ >> key) {
        if (key==_uid){
           return value;}
      }
    }
  }
  return string(); 
  }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { return 0; }

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

// run linux commad line and get the outputs
// https://stackoverflow.com/questions/478898/how-do-i-execute-a-command-and-get-the-output-of-the-command-within-c-using-po

std::string exec(const char* cmd) {
  std::array<char, 128> buffer;
  std::string result;
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
  if (!pipe) {
    throw std::runtime_error("popen() failed!");
  }
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }
  return result;
}