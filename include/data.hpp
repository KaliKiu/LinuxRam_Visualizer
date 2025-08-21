#ifndef DATA
#define DATA
#include <unordered_map>

class Data{
    public:
        Data();
        ~Data();
        static constexpr const char* MEMINFO_COPY_PATH = "../data/meminfo_copy";
        static constexpr const char* MEMINFO_PATH = "/proc/meminfo";
        static constexpr const char* MEMTOTAL = "MemTotal";
        static constexpr const char* MEMFREE = "MemFree";
        static constexpr const char* MEMCACKED = "Cached";
        std::unordered_map<std::string, int32_t> parseMeminfo();
    private:
        struct Meminfo{
            uint32_t memtotal;
            uint32_t memfree;
            uint32_t memcached;
        };

};

#endif