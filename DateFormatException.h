#ifndef HW2_DATEFORMATEXCEPTION_H
#define HW2_DATEFORMATEXCEPTION_H
#include <iostream>
#include <exception>
using namespace std;

class DateFormatException : public exception {
private:
    string message;
public:
    explicit DateFormatException(string message) : message(std::move(message)) {}
    const char * what() const noexcept override {
        return message.c_str();
    }
};

#endif //HW2_DATEFORMATEXCEPTION_H
