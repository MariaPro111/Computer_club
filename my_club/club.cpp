#include "club.h"

Club::Club(int64_t tables_count, Time time_start, Time time_end,
           int64_t hour_cost)
    : tables_count_(tables_count), time_start_(time_start), time_end_(time_end),
      free_tables_count_(tables_count), hour_cost_(hour_cost){};

void Club::ID_1(Event event) {
  if (clients_map_.find(event.name_) != clients_map_.end()) {
    std::vector<std::string> param = {event.parameters_[0], "13",
                                      "YouShallNotPass"};
    event_queue_.emplace(param, tables_count_);
  } else if (!event.time_.InInterval(time_start_, time_end_)) {
    std::vector<std::string> param = {event.parameters_[0], "13", "NotOpenYet"};
    event_queue_.emplace(param, tables_count_);
  } else {
    Client client;
    client.name_ = event.name_;
    client.table_number_ = event.table_number_;
    clients_map_[event.name_] = client;
  }
}

void Club::ID_2(Event event) {
  if (!tables_[event.table_number_].if_free_) {
    std::vector<std::string> param = {event.parameters_[0], "13",
                                      "PlaceIsBusy"};
    event_queue_.emplace(param, tables_count_);
  } else if (clients_map_.find(event.name_) == clients_map_.end()) {
    std::vector<std::string> param = {event.parameters_[0], "13",
                                      "ClientUnknown"};
    event_queue_.emplace(param, tables_count_);
  } else {
    if (!client_waiting_queue_.empty() &&
        (client_waiting_queue_.front().name_ == event.name_)) {
      client_waiting_queue_.pop();
    }
    if (clients_map_[event.name_].have_table) {
      int64_t number_now = clients_map_[event.name_].table_number_;
      tables_[number_now].if_free_ = true;
      free_tables_count_ += 1;
    }
    clients_map_[event.name_].table_number_ = event.table_number_;
    clients_map_[event.name_].have_table = true;
    tables_[event.table_number_].if_free_ = false;
    free_tables_count_ -= 1;
    tables_[event.table_number_].time_start_ = event.time_;
  }
}

void Club::ID_3(Event event) {
  if (free_tables_count_ != 0) {
    std::vector<std::string> param = {event.parameters_[0], "13",
                                      "ICanWaitNoLonger!"};
    event_queue_.emplace(param, tables_count_);
    Client client;
    client.name_ = event.name_;
    client.table_number_ = event.table_number_;
    client.have_table = false;
    client_waiting_queue_.push(client);
  } else if (client_waiting_queue_.size() == tables_count_) {
    std::vector<std::string> param = {event.parameters_[0], "11", event.name_};
    event_queue_.emplace(param, tables_count_);
  } else {
    Client client;
    client.name_ = event.name_;
    client.table_number_ = event.table_number_;
    client.have_table = false;
    client_waiting_queue_.push(client);
  }
}

void Club::ID_4(Event event) {
  if (clients_map_.find(event.name_) == clients_map_.end()) {
    std::vector<std::string> param = {event.parameters_[0], "13",
                                      "ClientUnknown"};
    event_queue_.emplace(param, tables_count_);
  } else {
    if (clients_map_[event.name_].table_number_ != -1) {
      int64_t number_now = clients_map_[event.name_].table_number_;
      tables_[number_now].if_free_ = true;
      free_tables_count_ += 1;
      Time dif_time = tables_[number_now].time_start_.Dif(event.time_);
      tables_[number_now].busy_time_.Sum(dif_time);
      tables_[number_now].income_ += hour_cost_ * dif_time.hours_;
      if (dif_time.minutes_ > 0) {
        tables_[number_now].income_ += hour_cost_;
      }
      if (!client_waiting_queue_.empty()) {
        tables_[number_now].if_free_ = false;
        free_tables_count_ -= 1;
        Client client = client_waiting_queue_.front();
        client_waiting_queue_.pop();
        client.table_number_ = number_now;
        client.have_table = true;
        clients_map_[client.name_] = client;
        std::vector<std::string> param = {event.parameters_[0], "12",
                                          client.name_,
                                          std::to_string(client.table_number_)};
        event_queue_.emplace(param, tables_count_);
        tables_[number_now].time_start_ = event.time_;
      }
    }
    clients_map_.erase(event.name_);
  }
}

void Club::ID_11(Event event) { clients_map_.erase(event.name_); }

void Club::RunEvents() {
  time_start_.PrintTime();
  std::cout << '\n';
  auto iter = events_.begin();
  while (iter != events_.end() || !event_queue_.empty()) {
    if (event_queue_.empty()) {
      event_queue_.push(*iter);
      ++iter;
    }
    Event current_event = event_queue_.front();
    event_queue_.pop();
    current_event.PrintEvent();
    if (current_event.id_ == 1) {
      ID_1(current_event);
    } else if (current_event.id_ == 2) {
      ID_2(current_event);
    } else if (current_event.id_ == 3) {
      ID_3(current_event);
    } else if (current_event.id_ == 4) {
      ID_4(current_event);
    } else if (current_event.id_ == 11) {
      ID_11(current_event);
    }
  }
  if (!clients_map_.empty()) {
    for (auto client : clients_map_) {
      if (client.second.have_table) {
        int64_t number_now = client.second.table_number_;
        Time dif_time = tables_[number_now].time_start_.Dif(time_end_);
        tables_[number_now].busy_time_.Sum(dif_time);
        tables_[number_now].income_ += hour_cost_ * dif_time.hours_;
        if (dif_time.minutes_ > 0) {
          tables_[number_now].income_ += hour_cost_;
        }
      }
      Event event;
      event.name_ = client.first;
      event.id_ = 11;
      event.time_ = time_end_;
      event.PrintEvent();
    }
  }
  time_end_.PrintTime();
  std::cout << '\n';
  for (auto table : tables_) {
    std::cout << table.first << ' ' << table.second.income_ << ' ';
    table.second.busy_time_.PrintTime();
    std::cout << '\n';
  }
}
