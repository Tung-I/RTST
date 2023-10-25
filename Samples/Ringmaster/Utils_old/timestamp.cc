#include <chrono>
#include "timestamp.hh"

using namespace std;
using namespace chrono;

uint64_t timestamp_ns()
{
  return duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
}

uint64_t timestamp_us()
{
  return duration_cast<microseconds>(system_clock::now().time_since_epoch()).count();
}

uint64_t timestamp_ms()
{
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}
