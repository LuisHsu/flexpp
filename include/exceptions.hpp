#ifndef Flex_exceptions
#define Flex_exceptions

#include <stdexcept>
#include <string>

namespace Flex {

struct Exception : public std::exception {
    Exception(std::string msg) : std::exception(), msg(msg){}
    const char* what() const noexcept {
        return msg.c_str();
    }
private:
    std::string msg;
};

struct Fatal : public Exception {
    using Exception::Exception;
};

struct Error : public Exception {
    using Exception::Exception;
};

void Warning(std::string msg);
void Warning(std::string msg, size_t line);

} // namespace Flex

#endif