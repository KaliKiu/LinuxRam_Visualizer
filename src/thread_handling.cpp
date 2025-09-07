#include <iostream>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <memory>
#include "../include/thread_handling.hpp"
#include "../include/data.hpp"


namespace Thread{
    void threadHandling(Data* data){
        std::mutex meminfo_mutex;
        std::mutex VPage_map_mutex;

        std::thread meminfo_fetch([&data,&meminfo_mutex] {
                        while(true){
                            data->parseMeminfo(meminfo_mutex);
                            std::this_thread::sleep_for(std::chrono::seconds(1));
                        }
                        });
        meminfo_fetch.detach();

        std::thread fetch_pid_data([&data,&VPage_map_mutex]{
                        while(true){
                            std::vector<std::string> pids = Data::getPid();
                            int count = 0;
                            std::vector<std::thread> threads;
                            for(const std::string &pid : pids){
                                threads.emplace_back([&data, &VPage_map_mutex,&pid, count]{
                                    data->parsePidMap(VPage_map_mutex,pid,count);
                                });
                            }
                            for(auto &t : threads) t.join();

                            std::this_thread::sleep_for(std::chrono::seconds(1));
                        }
                        });
        fetch_pid_data.detach();
        while(true){}
        /*int count=0;
        while(true){printf("\nCOUNT: %d\nMemTotal: %d\nMemFree: %d\n",count,
                    data->meminfo_struct->memtotal,data->meminfo_struct->memfree);
                    count++;
            {   
                auto VPage_map_ptr = data->VPage_map;
                std::lock_guard<std::mutex> lock(VPage_map_mutex);
                for(auto &t : *data->VPage_map){
                    printf("\n%d: 0x-0x",t.first,(*t.second)[0]->start_Vaddr,(*t.second)[0]->end_Vaddr);
                }
            }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }*/
}
}