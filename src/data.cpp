#include <unordered_map>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <mutex>
#include <dirent.h>
#include "../include/data.hpp"

    Data::Data(){
       meminfo_struct = new Meminfo(); 
    }

    Data::~Data(){
        delete meminfo_struct;
    }
    void Data::parseMeminfo(std::mutex& meminfo_mutex){
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
            //printf("\n%s: %d kb MAP : %d",key.c_str(),value,meminfo_map[key]);
        }
        meminfo.close();
        std::lock_guard<std::mutex> lock(meminfo_mutex);
        this->meminfo_struct->memtotal = meminfo_map[std::string(MEMTOTAL)];
        this->meminfo_struct->memfree = meminfo_map[std::string(MEMFREE)];

    }
    void Data::parsePidMaps(std::mutex &pidmap_vector_mutex, int count){
        return;
    }

    std::vector<std::string> Data::getPid(){
        const char* procDir = "/proc";
        DIR* dir = opendir(procDir);
        if (!dir) {
            perror("opendir");
            throw 0;
        }
        std::vector<std::string> pids;
        struct dirent* entry;

        auto isNumber = [&](const std::string& s){
            for (char c : s)
                if (!std::isdigit(c)) return false;
            return true;
        };

        while ((entry = readdir(dir)) != nullptr) {
            std::string name(entry->d_name);
            if (isNumber(name)) {
                pids.push_back(name);
            }
        }

        closedir(dir);
        return pids;
    }
