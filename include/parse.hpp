#ifndef PARSE
#define PARSE

#include <unordered_map>

namespace Parse{
    std::unordered_map<std::string, int32_t> parseMeminfo(const char* path);
}

#endif