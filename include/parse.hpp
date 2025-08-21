#ifndef PARSE
#define PARSE

#include <unordered_map>

namespace Parse{
    extern const char* MEMINFO_PATH;
    std::unordered_map<std::string, int32_t> parseMeminfo(const char* PATH);
}

#endif