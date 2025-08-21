#include <iostream>
#include <fstream>
#include <filesystem>
#include "../include/parse.hpp";

constexpr const char* MEMINFO_COPY = "../data/meminfo_copy";
int main(){
    try{
    Parse::parseMeminfo(MEMINFO_COPY);
    }catch(const std::filesystem::filesystem_error& e){

    }catch(const std::invalid_argument& e){

    }
}