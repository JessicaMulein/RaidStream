#ifndef RAIDSTREAM_RAIDSTREAM_EXCEPTION_HPP
#define RAIDSTREAM_RAIDSTREAM_EXCEPTION_HPP

#include <stdexcept>

namespace RaidStream {
    class Exception : public std::runtime_error {
    public:
        Exception(const std::string& what_arg) : std::runtime_error(what_arg) {}
        Exception(const char* what_arg) : std::runtime_error(what_arg) {}
    };
}

#endif //RAIDSTREAM_RAIDSTREAM_EXCEPTION_HPP
