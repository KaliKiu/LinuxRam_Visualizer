#include<unordered_map>
#include<iostream>
#include<filesystem>
#include <fstream>
#include <string>


namespace Parse{
    constexpr const char* MEMINFO_PATH = "/proc/meminfo";
    std::unordered_map<std::string, uint32_t> parseMeminfo(const char*& MEMINFO_COPY_PATH){
        std::unordered_map<std::string,uint32_t> meminfo_map;
        std::filesystem::copy_file(Parse::MEMINFO_PATH,MEMINFO_COPY_PATH,std::filesystem::copy_options::overwrite_existing);
        std::ifstream meminfo(MEMINFO_COPY_PATH);
        
        std::string line;
        while(std::getline(meminfo,line)){
            //get key;
            size_t colPos = line.find(':');
            std::string key= line.substr(0,colPos);
            std::string value_str=line.substr(colPos+1);
            value_str.erase(value_str.length()-3);
            int value = std::stoi(value_str);
        }
    }
}