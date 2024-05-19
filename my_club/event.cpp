#include "event.h"

bool GetNumber(std::string &str, int64_t &result) {
  try {
    size_t pos;
    result = std::stoll(str, &pos);
    if (pos != str.size()) {
      return false;
    }
    return true;
  } catch (const std::invalid_argument &e) {
    return false;
  }
}

bool CheckName(std::string &name) {
  for (char c : name) {
    if (c != '-' && c != '_' &&
        !(std::isdigit(c) || (std::islower(c) && std::isalpha(c)))) {
      return false;
    }
  }
  return true;
}

Event::Event(std::vector<std::string> &parameters, int64_t tables_count)
    : parameters_(parameters), tables_count_(tables_count) {
  int64_t id;
  Time time;
  int64_t table_number = 0;

  if ((parameters_.size() < 3 || parameters_.size() > 4 ||
       !GetTime(parameters_[0], time) || !GetNumber(parameters_[1], id)) ||
      (parameters_.size() == 3 && id == 2)) {
    PrintEventFail();
    exit(0);
  } else if ((id != 13) && !CheckName(parameters_[2])) {
    PrintEventFail();
    exit(0);
  } else if ((parameters_.size() == 4) &&
             (!GetNumber(parameters_[3], table_number) ||
              (table_number > tables_count_))) {
    PrintEventFail();
    exit(0);
  }
  id_ = static_cast<int>(id);
  time_ = time;
  table_number_ = table_number;
  name_ = parameters_[2];
}

void Event::PrintEvent() const {
  time_.PrintTime();
  std::cout << ' ' << id_ << ' ' << name_;
  if (id_ == 2 || id_ == 12) {
    std::cout << ' ' << table_number_;
  }
  std::cout << '\n';
}
void Event::PrintEventFail() const {
  for (const auto &word : parameters_) {
    std::cout << word << ' ';
  }
  std::cout << '\n';
}

