#include <iostream>
#include <thread>
#include "../include/fetch.hpp"
#include "../include/parse.hpp"

    Fetch::Fetch(){
        
    }
    Fetch::~Fetch(){
        this->meminfo_map.clear();
    }
    void Fetch::fetchMeminfo(const char* MEMINFO_COPY){
        
        this->meminfo_map= Parse::parseMeminfo(MEMINFO_COPY);
    }
