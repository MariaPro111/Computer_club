#pragma once

#include "table.h"
#include <iostream>
#include <string>
#include <vector>

bool GetNumber(std::string &str, int64_t &result);

bool CheckName(std::string &name);

class Event {
public:
  Event() = default;
  Event(std::vector<std::string> &parameters, int64_t tables_count);

  void PrintEvent() const;
  void PrintEventFail() const;

  int id_;
  Time time_;
  int64_t table_number_;
  int64_t tables_count_;
  std::string name_;
  std::vector<std::string> parameters_;
};
