#include <iostream>
#include <thread>
#include <mutex>
#include <unordered_map>
#include "../include/thread_handling.hpp"
#include "../include/data.hpp"


namespace Thread{
    void threadHandling(Data* data){
        //lambda function to call
        std::thread meminfo_fetch([&] {
                        while(true){
                            std::unordered_map<std::string,int> map = data->parseMeminfo();
                            
                            std::this_thread::sleep_for(std::chrono::seconds(1));
                        }
                        });
        meminfo_fetch.join();
    }
}