#ifndef DATA
#define DATA
#include <unordered_map>
#include <map>
#include <mutex>
#include <vector>
#include <cstdint>
#include <memory>
class Data{
    public:
        //CONSTRUCTOR
        Data(){
            meminfo_struct = new Meminfo();
            VPage_map = std::make_shared<std::map<uint32_t, std::shared_ptr<std::vector<std::shared_ptr<VPage>>>>>();
            }
        ~Data(){delete meminfo_struct;};

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
        void parsePidMap(const std::string pid,int count);
        static std::vector<std::string> getPid();

        //DATA
        struct Meminfo{
            uint32_t memtotal;
            uint32_t memfree;
        };
        Meminfo* meminfo_struct;
        
        struct VPage{
            uintptr_t start_Vaddr;
            uintptr_t end_Vaddr;
            uint32_t inode;
            std::string path_name;
        };
        //vector filled with virtual address mapped per pid in smart pointer
        std::shared_ptr<std::map<uint32_t,std::shared_ptr<std::vector<std::shared_ptr<VPage>>>>> VPage_map;
};
#endif