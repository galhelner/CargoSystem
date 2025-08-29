#ifndef HW2_PORT_H
#define HW2_PORT_H
#include <iostream>

class Port {
private:
    std::string name;
public:
    explicit Port(std::string name) : name(std::move(name)) {}
    std::string getName() const { return name; }
    bool operator==(const Port& other) const {
        return this->name == other.name;
    }
};


#endif //HW2_PORT_H
