#include "Date.h"
#include <sstream>
#include <iomanip>
#include "DateFormatException.h"

Date::Date(std::string stringDate) : stringDate(std::move(stringDate)) {
    char slash, colon;
    std::istringstream ss(this->stringDate);

    ss >> std::setw(2) >> this->day >> slash
       >> std::setw(2) >> this->month;

    if (ss.fail() || slash != '/') {
        throw DateFormatException("");
    }

    ss.ignore(1); // skip the space

    ss >> std::setw(2) >> this->hour >> colon
       >> std::setw(2) >> this->minute;

    if (ss.fail() || colon != ':') {
        throw DateFormatException("");
    }

    // Check for extra characters
    char extra;
    if (ss >> extra) {
        throw DateFormatException("");
    }
}

int Date::toMinutes() const {
    const int daysInMonthAt2023[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int totalDays = 0;

    for (int i = 0; i < month - 1; ++i) {
        totalDays += daysInMonthAt2023[i];
    }

    totalDays += (day - 1);

    return totalDays * 24 * 60 + hour * 60 + minute;
}

int Date::minutesSince(std::shared_ptr<Date> other) const {
    return this->toMinutes() - other->toMinutes();
}

std::string Date::getAsString() const {
    return this->stringDate;
}