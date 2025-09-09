#ifndef THREAD_HANDLING
#define THREAD_HANDLING
#include <iostream>
#include <mutex>
#include "../include/data.hpp"

namespace Thread{
   void threadHandling(std::shared_ptr<Data> data); 
}
#endif