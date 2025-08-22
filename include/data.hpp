#ifndef DATA
#define DATA
#include <unordered_map>
#include <map>
#include <mutex>
#include <vector>
class Data{
    public:
        //CONSTRUCTOR
        Data();
        ~Data();

        //CONSTS
        static constexpr const char* MEMINFO_COPY_PATH = "../data/meminfo_copy";
        static constexpr const char* MEMINFO_PATH = "/proc/meminfo";
        static constexpr const char* MEMTOTAL = "MemTotal";
        static constexpr const char* MEMFREE = "MemFree";
        static constexpr const char* MEMCACHED = "Cached";
        static constexpr const char* BUFFERS = "Buffers";
        static constexpr const char* ACTIVE_ANON = "Active(anon)";
        static constexpr const char* ACTIVE_FILE = "Active(file)";
        static constexpr const char* INACTIVE_ANON = "Inactive(anon)";
        static constexpr const char* INACTIVE_FILE = "Inactive(file)";
        static constexpr const char* SLAB = "Slab";

        //FUNCTIONS
        void parseMeminfo(std::mutex  &meminfo_mutex);
        void parsePidMaps(std::mutex &pidmap_vector_mutex,int count);
        static std::vector<std::string> getPid();

        //DATA
        struct Meminfo{
            uint32_t memtotal;
            uint32_t memfree;
        };
        Meminfo* meminfo_struct;
        //vector filled with virtual addressed mapped per pid
        std::vector<std::map<uint32_t,uint32_t>> pidsMap;

};

#endif