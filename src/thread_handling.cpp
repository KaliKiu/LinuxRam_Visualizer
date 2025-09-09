#include <iostream>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <memory>
#include <condition_variable>
#include "../include/thread_handling.hpp"
#include "../include/data.hpp"


namespace Thread{
    void threadHandling(std::shared_ptr<Data> data){
        std::mutex meminfo_mutex;
        std::mutex VAddress_map_mutex;
        std::mutex PageMap_mutex;
        std::condition_variable cv_fetch_thread;
        bool cv_fetch_thread_ready = false;
        bool debug_ready = false;

        std::thread meminfo_fetch([data,&meminfo_mutex] {
                        while(true){
                            data->parseMeminfo(meminfo_mutex);
                            std::this_thread::sleep_for(std::chrono::seconds(1));
                        }
                        });
        meminfo_fetch.detach();

        std::thread fetch_pid_data([data,&VAddress_map_mutex,&PageMap_mutex,&cv_fetch_thread,&cv_fetch_thread_ready,&debug_ready]{
                        while(true){
                            {
                            std::unique_lock<std::mutex> lock(PageMap_mutex);
                            auto pids = Data::getPid();
                            if(data->pids == nullptr)continue;
                            data->pids = pids;
                            int count = 0;
                            std::vector<std::thread> threads;
                            
                            for(const std::string &pid : *(data->pids)){
                                threads.emplace_back([data,pid, count]{
                                    data->parsePidMap(pid,count);

                                });
                            }
                            for(auto &t : threads) t.join();
                            //fetch pagemap everything oowowowowo
                            cv_fetch_thread_ready=true;
                            debug_ready = true;
                            cv_fetch_thread.notify_all();
                            }
                            
                            std::this_thread::sleep_for(std::chrono::seconds(1));
                        }
                        });
        fetch_pid_data.detach();

        std::thread fetch_pid_PageMap_data([data,&VAddress_map_mutex,&PageMap_mutex,&cv_fetch_thread,&cv_fetch_thread_ready]{
                        while(true){
                            
                            std::unique_lock<std::mutex> lock(VAddress_map_mutex);
                            cv_fetch_thread.wait(lock,[&cv_fetch_thread_ready]{return cv_fetch_thread_ready;});cv_fetch_thread_ready=false;
                            if(data->pids == nullptr)continue;
                            auto pids = data->pids;
                            std::vector<std::thread> threads; 
                            for(auto &t : *data->VPage_map){
                                data->parsePidPageMap(t.first,(*t.second),PageMap_mutex);
                            }
                            lock.unlock();
                            std::this_thread::sleep_for(std::chrono::seconds(1));
                        }
        });

        //while(true){std::cout<<"a"<<std::endl;std::this_thread::sleep_for(std::chrono::seconds(1));}
        
        //debugging stuff ahead!!!
        int count=0;
        while(true){printf("\nCOUNT: %d\nMemTotal: %d\nMemFree: %d\n",count,
                    data->meminfo_struct->memtotal,data->meminfo_struct->memfree);
                    count++;
            {   
                auto VPage_map_ptr = data->VPage_map;
                std::unique_lock<std::mutex> lock(VAddress_map_mutex);
                cv_fetch_thread.wait(lock,[&debug_ready]{return debug_ready;});debug_ready = false;
                
                
                /*auto pidVpage = *data->VPage_map->begin()->second;
                for(int i{0}; i<pidVpage.size();i++){
                    if(!pidVpage[i])std::cout<<"null"<<std::endl;continue;
                    printf("%lx-%lx name: %s",pidVpage[i]->start_Vaddr,pidVpage[i]->end_Vaddr,pidVpage[i]->path_name);
                }*/

                for(auto &t : *data->VPage_map){
                    if(!t.second || t.second->empty()) continue;
                    printf("\n%d: %lx-%lx vectorsize %d",t.first,(*t.second)[0]->start_Vaddr,(*t.second)[0]->end_Vaddr,(*t.second).size());
                    
                }
            }
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
}