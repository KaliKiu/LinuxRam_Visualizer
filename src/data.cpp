#include <unordered_map>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <mutex>
#include <dirent.h>
#include <memory>
#include "../include/data.hpp"

    void Data::parseMeminfo(std::mutex& meminfo_mutex){
        std::unordered_map<std::string,int32_t> meminfo_map;
        std::ifstream meminfo(MEMINFO_PATH);
        std::stringstream buffer;
        buffer <<meminfo.rdbuf();
        std::string line;
        while(std::getline(buffer,line)){
            //get key;
            size_t colPos = line.find(':');
            std::string key= line.substr(0,colPos);
            std::string value_str=line.substr(colPos+1);
            // remove trailing "kB" if present
            if(value_str.size() >= 2 && value_str.substr(value_str.size()-2) == "kB") {
                value_str.erase(value_str.size()-2);
            }
            auto trim = [](std::string& s) {
                size_t start = s.find_first_not_of(" \t");
                size_t end = s.find_last_not_of(" \t");
                if (start == std::string::npos) { s = ""; return; }
                    s = s.substr(start, end - start + 1);
                };
            trim(value_str);
            int32_t value = std::stoul(value_str);
            meminfo_map[key]=value;
            //printf("\n%s: %d kb MAP : %d",key.c_str(),value,meminfo_map[key]);
        }
        meminfo.close();
        std::lock_guard<std::mutex> lock(meminfo_mutex);
        this->meminfo_struct->memtotal = meminfo_map[std::string(MEMTOTAL)];
        this->meminfo_struct->memfree = meminfo_map[std::string(MEMFREE)];

    }
    void Data::parsePidMap(std::mutex &VPage_map_mutex,const std::string pid, int count){
        auto PidPages = std::make_shared<std::vector<std::shared_ptr<VPage>>>();
        std::string pidmem_path = "/proc/"+pid+"/maps";
        std::stringstream buffer;
        std::ifstream pidmap(pidmem_path);
        buffer <<pidmap.rdbuf();
        std::string line;
        //! istringstream is better YES, but i wanted to try myself..
        while(std::getline(buffer,line)){
            if(line ==""){
                continue;
            }
            size_t startEndPointer = line.find('-');
            size_t WhiteSpace = line.find(' ');
            size_t WhiteSpace2 = line.find(' ',WhiteSpace+1);
            auto forwardWhiteSpace = [&WhiteSpace,&WhiteSpace2](std::string &line,int&& times){
                for(;times==0;times--){
                WhiteSpace = WhiteSpace2+1;
                if(line.find(' ',WhiteSpace)!=std::string::npos){
                    WhiteSpace2 = line.find(' ',WhiteSpace);
                }else{
                    WhiteSpace2 = 0;
                }
                }
            };
            std::string start_address = line.substr(0,startEndPointer);
            std::string end_address = line.substr(startEndPointer+1,WhiteSpace);
            //skip perms,offset,dev| check for indeo if = 0 (stack,heap etc);
            forwardWhiteSpace(line,4);
            std::string inode = line.substr(WhiteSpace,WhiteSpace2);
            forwardWhiteSpace(line,1);
            std::string path_name = line.substr(WhiteSpace);

            auto page = std::make_shared<VPage>();
            page->start_Vaddr = static_cast<uintptr_t>(std::stoull(start_address,nullptr,16));
            page->end_Vaddr = static_cast<uintptr_t>(std::stoull(end_address,nullptr,16));
            page->inode = std::stoul(inode);
            page->path_name = path_name;
            PidPages->push_back(std::move(page));
        }
        std::lock_guard<std::mutex> lock(VPage_map_mutex);
        this->VPage_map->emplace(std::stol(pid),std::move(PidPages));
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
