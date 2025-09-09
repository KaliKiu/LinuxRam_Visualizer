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
#include <sstream>
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
    void Data::parsePidMap(const std::string pid, int count){
        auto PidPages = std::make_shared<std::vector<std::shared_ptr<VPage>>>();
        std::string pidmem_path = "/proc/"+pid+"/maps";
        std::stringstream buffer;
        std::ifstream pidmap(pidmem_path);
        buffer <<pidmap.rdbuf();
        std::string line;
        //! istringstream is better YES, but i wanted to try myself..
        while(std::getline(buffer,line)){
            if(line.empty()){
                continue;
            }
            std::istringstream iss(line);
            std::string Vaddr,start_Vaddr,end_Vaddr,perms,dev,path_name;
            uint32_t offset,inode;
            iss >>Vaddr >>perms >>offset >>dev >>inode >>path_name;
            size_t pos = Vaddr.find('-');
            start_Vaddr = Vaddr.substr(0,pos);
            end_Vaddr = Vaddr.substr(pos+1);
            auto page = std::make_shared<VPage>();
            page->start_Vaddr = static_cast<uintptr_t>(std::stoull(start_Vaddr,nullptr,16));
            page->end_Vaddr = static_cast<uintptr_t>(std::stoull(end_Vaddr,nullptr,16));
            //page->inode = std::stoul(inode);
            page->path_name = path_name;
            PidPages->push_back(std::move(page));
        }
        this->VPage_map->emplace(std::stol(pid),std::move(PidPages));
    }
    void Data::parsePidPageMap(const std::string pid, std::mutex PageMap_mutex){
        const std::string path =  "/proc"+pid+"/pagemap";
        std::ifstream file(path,std::ios::binary);
        if(!file){
            std::cout <<"parsePidPageMap fail";
            throw 1;
        }

        
    }

    std::shared_ptr<std::vector<std::string>> Data::getPid(){
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
        return std::make_shared<std::vector<std::string>>(std::move(pids));
    }
