#include "time.h"
#include <regex>

bool GetTime(std::string &str, Time &time) {
  std::istringstream ss(str);
  std::regex format(R"(^\d{2}:\d{2}$)");
  if (!std::regex_match(str, format)) {
    return false;
  }
  char delimiter;
  int hours;
  int minutes;
  if ((ss >> hours >> delimiter >> minutes) &&
      (delimiter == ':' && hours >= 0 && hours < 24 && minutes >= 0 &&
       minutes < 60)) {
    time.hours_ = hours;
    time.minutes_ = minutes;
    return true;
  }
  return false;
};

bool Time::operator>=(const Time &other) const {
  return (hours_ > other.hours_) ||
         (hours_ == other.hours_ && minutes_ >= other.minutes_);
}

bool Time::operator<=(const Time &other) const {
  return (hours_ < other.hours_) ||
         ((hours_ == other.hours_) && (minutes_ <= other.minutes_));
}

bool Time::operator<(const Time &other) const {
  return (hours_ < other.hours_) ||
         (hours_ == other.hours_ && minutes_ < other.minutes_);
}

void Time::Sum(const Time &other) {
  hours_ += other.hours_;
  minutes_ += other.minutes_;
  if (minutes_ >= 60) {
    hours_ += minutes_ / 60;
    minutes_ = minutes_ % 60;
  }
}

Time Time::Dif(const Time &other) {
  Time time;
  time.hours_ = other.hours_ - hours_;
  time.minutes_ = other.minutes_ - minutes_;
  if (time.minutes_ < 0) {
    time.hours_ -= 1;
    time.minutes_ = time.minutes_ + 60;
  }
  return time;
}

bool Time::InInterval(Time start, Time end) {
  return (start <= *this) && (end >= *this);
}

void Time::PrintTime() const {
  std::ostringstream oss;
  std::cout << std::setw(2) << std::setfill('0') << hours_ << ":"
            << std::setw(2) << std::setfill('0') << minutes_;
}
