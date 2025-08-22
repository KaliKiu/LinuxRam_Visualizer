#include <iostream>
#include <thread>
#include <mutex>
#include <unordered_map>
#include "../include/thread_handling.hpp"
#include "../include/data.hpp"


namespace Thread{
    void threadHandling(Data* data){
        std::mutex meminfo_mutex;
        //lambda function to call
        std::thread meminfo_fetch([&data,&meminfo_mutex] {
                        while(true){
                            data->parseMeminfo(meminfo_mutex);
                            std::this_thread::sleep_for(std::chrono::seconds(1));
                        }
                        });
        meminfo_fetch.detach();
        int count=0;
        while(true){printf("\nCOUNT: %d\nMemTotal: %d\nMemFree: %d\n MemCached: %d\n MemBuffers: %d\n",count,
                                                                                                        data->meminfo_struct->memtotal,
                                                                                                        data->meminfo_struct->memfree,
                                                                                                        data->meminfo_struct->memcached,
                                                                                                        data->meminfo_struct->membuffers);count++;std::this_thread::sleep_for(std::chrono::seconds(1));}
    }
}