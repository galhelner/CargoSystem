#ifndef HW2_FILEERROREXCEPTION_H
#define HW2_FILEERROREXCEPTION_H
#include <iostream>
#include <exception>

class FileErrorException : public std::exception {
private:
    std::string message;
public:
    explicit FileErrorException(std::string message) : message(std::move(message)) {}
    const char * what() const noexcept override {
        return message.c_str();
    }
};

#endif //HW2_FILEERROREXCEPTION_H
