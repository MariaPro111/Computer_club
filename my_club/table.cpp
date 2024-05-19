#include "table.h"
#include <cstdint>

Table::Table(int64_t number) : number_(number){};

bool Table::operator<(const Table &other) const {
  return number_ < other.number_;
}
