#ifndef HW2_DATE_H
#define HW2_DATE_H
#include <iostream>
#include <memory>

/**
 * This class represent a date object in my project.
 * We assume that all the dates in the project are on the same year.
 */
class Date {
private:
    std::string stringDate;
    int day;
    int month;
    int hour;
    int minute;
public:
    explicit Date(std::string stringDate);
    int toMinutes() const;
    int minutesSince(std::shared_ptr<Date> other) const;
    std::string getAsString() const;
};


#endif //HW2_DATE_H
