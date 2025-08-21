#ifndef PARSE
#define PARSE

#include <unordered_map>

namespace Parse{
    extern const char* MEMINFO_PATH = "/proc/meminfo";
    std::unordered_map<std::string, uint32_t> parseMeminfo(const char* PATH);
}

#endif