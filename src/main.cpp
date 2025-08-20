#include <iostream>
#include <fstream>
#include <filesystem>

constexpr const char* MEMINFO = "/proc/meminfo";
constexpr const char* MEMINFO_COPY = "../data/meminfo_copy";
int main(){
    std::ifstream meminfo(MEMINFO);
    std::ofstream meminfo_copy(MEMINFO_COPY);
    std::filesystem::copy_file(MEMINFO,MEMINFO_COPY,std::filesystem::copy_options::overwrite_existing);
}