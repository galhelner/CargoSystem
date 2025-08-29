#ifndef HW2_SAILTIMESDATA_H
#define HW2_SAILTIMESDATA_H
#include <iostream>
#include <memory>
#include "Date.h"
using namespace std;

/**
 * A class that represent sail data as the times graph's edge weight.
 */
class SailTimesData {
private:
    shared_ptr<Date> startDate;
    shared_ptr<Date> endDate;
    int averageSailTime;
public:
    SailTimesData(shared_ptr<Date> startDate, shared_ptr<Date> endDate, int averageSailTime) {
        this->startDate = std::move(startDate);
        this->endDate = std::move(endDate);
        this->averageSailTime = averageSailTime;
    }
    shared_ptr<Date> getStartDate() const { return startDate; }
    shared_ptr<Date> getEndDate() const { return endDate; }
    int getAverageSailTime() const { return averageSailTime; }
    void setAverageSailTime(int newSailTime) { this->averageSailTime = newSailTime; }
};


#endif //HW2_SAILTIMESDATA_H
