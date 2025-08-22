#include <iostream>
#include <thread>
#include <mutex>
#include <unordered_map>
#include "../include/thread_handling.hpp"
#include "../include/data.hpp"


namespace Thread{
    void threadHandling(Data* data){
        std::mutex meminfo_mutex;
        std::mutex pidmap_vector_mutex;

        std::thread meminfo_fetch([&data,&meminfo_mutex] {
                        while(true){
                            data->parseMeminfo(meminfo_mutex);
                            std::this_thread::sleep_for(std::chrono::seconds(1));
                        }
                        });
        meminfo_fetch.detach();

        std::thread fetch_pid_data([&data,&pidmap_vector_mutex]{
                        while(true){
                            std::vector<std::string> pids = Data::getPid();
                            int count = 0;
                            std::vector<std::thread> threads;
                            for(const std::string& pid : pids){
                                threads.emplace_back([&data, &pidmap_vector_mutex, count]{
                                    data->parsePidMaps(pidmap_vector_mutex,count);
                                });
                            }
                            for(auto &t : threads) t.join();

                            std::this_thread::sleep_for(std::chrono::seconds(1));
                        }
                        });
        fetch_pid_data.detach();
        int count=0;
        while(true){printf("\nCOUNT: %d\nMemTotal: %d\nMemFree: %d\n",count,
                                                                        data->meminfo_struct->memtotal,
                                                                        data->meminfo_struct->memfree);count++;std::this_thread::sleep_for(std::chrono::seconds(1));}
    }
}