#include <iostream>
#include <fstream>
#include <filesystem>
#include <exception>
#include "../include/data.hpp"
#include "../include/thread_handling.hpp"


int main(){
    auto data = std::make_shared<Data>();
    
    try{
        Thread::threadHandling(data);
    }catch(const std::filesystem::filesystem_error& e){
        std::cerr <<"filesystem_error: " <<e.what() <<"\nexit code: "<<e.code();
    }catch(const std::invalid_argument& e){
        std::cerr<<"invalid_argument: " <<e.what();
    }catch(const std::system_error &e){
        std::cerr<<"system_error"<<e.code() <<"exit code: "<<e.code();
    }catch(...){
        std::cerr<<"U cooked man";
    }

}