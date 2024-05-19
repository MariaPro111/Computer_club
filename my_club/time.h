#pragma once

#include <iomanip>
#include <iostream>
#include <sstream>

struct Time {

  Time() = default;

  bool operator>=(const Time &other) const;

  bool operator<=(const Time &other) const;

  bool operator<(const Time &other) const;

  void Sum(const Time &other);

  Time Dif(const Time &other);
  bool InInterval(Time start, Time end);

  void PrintTime() const;
  int hours_ = 0;
  int minutes_ = 0;
};

bool GetTime(std::string &Str, Time &time);
