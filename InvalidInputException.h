#ifndef HW2_INVALIDINPUTEXCEPTION_H
#define HW2_INVALIDINPUTEXCEPTION_H
#include <iostream>
#include <exception>

class InvalidInputException : public std::exception {
private:
    std::string message;
public:
    explicit InvalidInputException(std::string message) : message(std::move(message)) {}
    const char * what() const noexcept override {
        return message.c_str();
    }
};

#endif //HW2_INVALIDINPUTEXCEPTION_H
