#pragma once
#include "time.h"

class Table {
public:
  Table() = default;
  explicit Table(int64_t number);

  bool operator<(const Table &other) const;

  int64_t number_ = 0;
  Time busy_time_;
  int64_t income_ = 0;
  Time time_start_;
  bool if_free_ = true;
};