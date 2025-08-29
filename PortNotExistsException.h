#ifndef HW2_PORTNOTEXISTSEXCEPTION_H
#define HW2_PORTNOTEXISTSEXCEPTION_H
#include <istream>
#include <exception>

class PortNotExistsException : public std::exception {
private:
    std::string message;
public:
    explicit PortNotExistsException(std::string message) : message(std::move(message)) {}
    const char * what() const noexcept override {
        return message.c_str();
    }
};

#endif //HW2_PORTNOTEXISTSEXCEPTION_H
