#ifndef DATA
#define DATA
#include <unordered_map>
#include <mutex>
class Data{
    public:
        Data();
        ~Data();
        static constexpr const char* MEMINFO_COPY_PATH = "../data/meminfo_copy";
        static constexpr const char* MEMINFO_PATH = "/proc/meminfo";
        static constexpr const char* MEMTOTAL = "MemTotal";
        static constexpr const char* MEMFREE = "MemFree";
        static constexpr const char* MEMCACHED = "Cached";
        static constexpr const char* BUFFERS = "Buffers";

        void parseMeminfo(std::mutex  &meminfo_mutex);

        //no swapped memory representation atm
        struct Meminfo{
            uint32_t memtotal;
            uint32_t memfree;
            uint32_t memcached;
            uint32_t membuffers;
        };
        Meminfo* meminfo_struct;

};

#endif