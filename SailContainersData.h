#ifndef HW2_SAILCONTAINERSDATA_H
#define HW2_SAILCONTAINERSDATA_H
#include <iostream>
#include "Date.h"
using namespace std;

/**
 * A class that represent sail data as the containers graph's edge weight.
 */
class SailContainersData {
private:
    shared_ptr<Date> startDate;
    shared_ptr<Date> endDate;
    int containers;
public:
    SailContainersData(shared_ptr<Date> startDate, shared_ptr<Date> endDate, int containers) {
        this->startDate = std::move(startDate);
        this->endDate = std::move(endDate);
        this->containers = containers;
    }
    shared_ptr<Date> getStartDate() const { return startDate; }
    shared_ptr<Date> getEndDate() const { return endDate; }
    int getContainers() const { return containers; }
};

#endif //HW2_SAILCONTAINERSDATA_H
