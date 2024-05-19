#pragma once

#include "event.h"
#include <iostream>
#include <map>
#include <queue>
#include <vector>

struct Client {
  std::string name_;
  int64_t table_number_ = -1;
  bool have_table = false;
};

class Club {
public:
  Club(int64_t tables_count, Time time_start, Time time_end, int64_t hour_cost);

  void ID_1(Event event);

  void ID_2(Event event);

  void ID_3(Event event);

  void ID_4(Event event);

  void ID_11(Event event);

  void RunEvents();

  std::map<int64_t, Table> tables_;
  std::vector<Event> events_;
  int64_t tables_count_ = 0;
  Time time_start_;
  Time time_end_;
  int64_t hour_cost_ = 0;
  int64_t free_tables_count_ = 0;
  std::map<std::string, Client> clients_map_;
  std::queue<Event> event_queue_;
  std::queue<Client> client_waiting_queue_;
};
