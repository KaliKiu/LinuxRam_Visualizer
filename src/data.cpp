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
    void Data::parsePidMaps(std::mutex &pidmap_VM_mutex,const std::string pid, int count){
        std::string pidmem_path = "/proc/"+pid+"/maps";
        std::stringstream buffer;
        std::ifstream pidmap(pidmem_path);
        buffer << pidmap;
        std::string line;
        while(std::getline(buffer,line)){
            size_t startEndPointer = line.find('-');
            size_t WhiteSpace = line.find(' ');
            size_t WhiteSpace2 = line.find(' ',Whitespace+1);
            auto forwardWhitespace = [&WhiteSpace,&Whitespace2](int &line,int times){
                for(;times==0;times--){
                WhiteSpace = WhiteSpace2+1;
                if(line.find(' ',WhiteSpace)!=std::string::npos){
                    WhiteSpace2 = line.find(' ',WhiteSpace);
                }else{
                    WhiteSpace2 = 0;
                }
                }
            }
            std::string start_address = line.substr(0,startEndPointer);
            std::string end_address = line.substr(startEndPointer+1,WhiteSpace);
            //skip perms,offset,dev. check for inode>0
            forwardWhiteSpace(line,4);
            std::string inode = line.substr(WhiteSpace,WhiteSpace2);
            forwardWhiteSpace(line,1);
            std::string path_name = line.substr(WhiteSpace);
            

            

            


        }
        if(pidmap_str==""){
            //no need, can iterate trough actually mapped pids (just for debugging reason rn)
            std::lock_guard<std::mutex> lock(pidmap_VM_mutex);
            //this->VM_map.emplace(std::stoul(pid),0);
            return;
        }
        size_t stop_pos = pidmap_str.find('-');
        std::string start_address = pidmap_str.substr(0,stop_pos);
        std::string end_address = pidmap_str.substr(stop_pos+1);
        auto vm_address = std::make_shared<VM_address_struct>();

        std::lock_guard<std::mutex> lock(pidmap_VM_mutex);
        vm_address->start_Vaddr = static_cast<uintptr_t>(std::stoull(start_address,nullptr,16));
        vm_address->end_Vaddr = static_cast<uintptr_t>(std::stoull(end_address,nullptr,16));
        //also pass ownership of struct to map
        this->Vpage.emplace(std::stoul(pid),std::move(vm_address));
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
