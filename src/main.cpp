#include <iostream>
#include <fstream>
#include <filesystem>
#include <exception>
#include "../include/parse.hpp"
#include "../include/fetch.hpp"

const char* MEMINFO_COPY = "../data/meminfo_copy";
int main(){
    Fetch* fetch = new Fetch();
    try{
        fetch->fetchMeminfo(MEMINFO_COPY);
    }catch(const std::filesystem::filesystem_error& e){
        std::cerr <<"filesystem_error: " <<e.what()
                    <<"\nexit code: "<<e.code();
    }catch(const std::invalid_argument& e){
        std::cerr<<"invalid argument: " <<e.what();
    }catch(...){
        std::cerr<<"U cooked man";
    }
    std::cout <<std::endl <<fetch->meminfo_map["MemTotal"];
}