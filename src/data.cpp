#include <unordered_map>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include "../include/data.hpp"

    Data::Data(){
        
    }
    Data::~Data(){
        
    }
    std::unordered_map<std::string, int32_t> Data::parseMeminfo(){
        std::unordered_map<std::string,int32_t> meminfo_map;
        std::filesystem::copy_file(MEMINFO_PATH,MEMINFO_COPY_PATH,std::filesystem::copy_options::overwrite_existing);
        std::filesystem::permissions(MEMINFO_COPY_PATH,std::filesystem::perms::all,std::filesystem::perm_options::add);
        std::ifstream meminfo(MEMINFO_COPY_PATH);
        
        std::string line;
        while(std::getline(meminfo,line)){
            //get key;
            size_t colPos = line.find(':');
            std::string key= line.substr(0,colPos);
            std::string value_str=line.substr(colPos+1);
            // remove trailing "kB" if present
            if(value_str.size() >= 2 && value_str.substr(value_str.size()-2) == "kB") {
                value_str.erase(value_str.size()-2);
            }
            int32_t value = std::stoul(value_str);
            meminfo_map[key]=value;
            printf("\n%s: %d kb MAP : %d",key.c_str(),value,meminfo_map[key]);
        }
        meminfo.close();
        return meminfo_map;
    }
