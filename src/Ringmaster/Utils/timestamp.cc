#include <chrono>
#include "timestamp.hh"

using namespace std;
using namespace chrono;


// functions that can return timestamp in nanoseconds, microseconds, or milliseconds
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

// uint64_t timestamp_ns()
// {
//   return system_clock::now().time_since_epoch() / 1ns;
// }

// uint64_t timestamp_us()
// {
//   return system_clock::now().time_since_epoch() / 1us;
// }

// uint64_t timestamp_ms()
// {
//   return system_clock::now().time_since_epoch() / 1ms;
// }
