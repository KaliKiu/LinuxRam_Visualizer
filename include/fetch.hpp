#ifndef FETCH
#define FETCH
#include <iostream>
#include <unordered_map>
class Fetch{
    public: 
        std::unordered_map<std::string,int32_t> meminfo_map;
        Fetch();
        ~Fetch();
        void fetchMeminfo(const char* MEMINFO_COPY);
};
#endif