#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "club.h"



void Read(std::ifstream& input_file, std::string& line){
    if(!std::getline(input_file, line)) {
        std::cout << "Not enough input data" << std::endl;
        exit(0);
    }
}

std::vector<std::string> Split(std::string& line) {
    std::vector<std::string> words;
    std::istringstream ss(line);
    std::string word;
    while (ss >> word) {
        words.push_back(word);
    }
    return words;
}



int main(int argc, char** argv) {
    if (argc < 2){
        std::cout<<"Input data error. Please add the input file."<<std::endl;
        return 0;
    }
    std::string filename = argv[1];
    std::ifstream input_file(filename);
    if (!input_file.is_open()) {
        std::cout<< "Failed to open file " << filename << std::endl;
        return 0;
    }

    std::string line;
    Read(input_file, line);
    int64_t table_count;
    if(!GetNumber(line,table_count) || table_count < 0){
        std::cout<<line<<std::endl;
        return 0;
    }

    Read(input_file, line);
    std::vector<std::string> time = Split(line);
    if(time.size() != 2){
        std::cout<<line<<std::endl;
        return 0;
    }
    Time time_start;
    Time time_end;
    if(!GetTime(time[0], time_start) || !GetTime(time[1], time_end) || (time_end < time_start)){
        std::cout<<line<< std::endl;
        return 0;
    }

    Read(input_file, line);
    int64_t hour_cost;
    if(!GetNumber(line,hour_cost) || hour_cost < 0){
        std::cout<<line<<std::endl;
        return 0;
    }

    Club club(table_count, time_start, time_end, hour_cost);
    while(std::getline(input_file, line)) {
        std::vector<std::string> vec_event = Split(line);
        Event event(vec_event, table_count);
        if (!club.events_.empty() && (event.time_ < club.events_.back().time_)){
            std::cout<<line<<std::endl;
            return 0;
        }
        club.events_.push_back(event);
    }
    input_file.close();
    club.RunEvents();

    return 0;
}
