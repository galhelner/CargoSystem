#include "ContainersGraph.h"
#include "PortNotExistsException.h"
#include <fstream>

shared_ptr<Port> ContainersGraph::findPortByName(const std::string &portName) {
    for (const auto& port : ports) {
        if (port->getName() == portName) {
            return port;
        }
    }
    return nullptr;
}

bool ContainersGraph::sailExists(shared_ptr<Port> srcPort, shared_ptr<Port> destPort, shared_ptr<Date> startDate,
                                 shared_ptr<Date> endDate) {
    for (const auto& sailTuple : sails) {
        const auto& foundedSrcPort = get<0>(sailTuple);
        const auto& foundedDestPort = get<1>(sailTuple);
        const auto& foundedSailData = get<2>(sailTuple);
        const auto& foundedStartDate = foundedSailData->getStartDate()->toMinutes();
        const auto& foundedEndDate = foundedSailData->getEndDate()->toMinutes();

        bool sameSrc = foundedSrcPort->getName() == srcPort->getName();
        bool sameDest = foundedDestPort->getName() == destPort->getName();
        bool sameStartDate = foundedStartDate == startDate->toMinutes();
        bool sameEndDate = foundedEndDate == endDate->toMinutes();

        // found edge with the same values
        if (sameSrc && sameDest && sameStartDate && sameEndDate) {
            return true;
        }
    }

    // no matching edge found
    return false;
}

void ContainersGraph::addPort(shared_ptr<Port> port) {
    this->ports.insert(std::move(port));
}

void ContainersGraph::addSail(shared_ptr<Port> srcPort, shared_ptr<Port> destPort, shared_ptr<Date> startDate,
                              shared_ptr<Date> endDate, int containers) {
    if (!sailExists(srcPort, destPort, startDate, endDate)) {
        shared_ptr<SailContainersData> sailData = std::make_shared<SailContainersData>(startDate, endDate, containers);
        sails.emplace_back(std::move(srcPort), std::move(destPort), std::move(sailData));
    }
}

vector<tuple<shared_ptr<Port>, int>> ContainersGraph::inboundContainers(const shared_ptr<Port>& port, const shared_ptr<Date>& date) {
    vector<tuple<shared_ptr<Port>, int>> inboundContainers;

    for (const auto& sailTuple : sails) {
        const auto& srcPort = get<0>(sailTuple);
        const auto& destPort = get<1>(sailTuple);
        const auto& sailData = get<2>(sailTuple);

        // sail from some port to the inbound port should end before the given time (containers arrived)
        bool beforeFinalDate = sailData->getEndDate()->toMinutes() <= date->toMinutes();

        if (destPort->getName() == port->getName() && beforeFinalDate) {
            inboundContainers.emplace_back(srcPort, sailData->getContainers());
        }
    }

    return inboundContainers;
}

vector<tuple<shared_ptr<Port>, int>>
ContainersGraph::outboundContainers(const shared_ptr<Port> &port, const shared_ptr<Date> &date) {
    vector<tuple<shared_ptr<Port>, int>> outboundContainers;

    for (const auto& sailTuple : sails) {
        const auto& srcPort = get<0>(sailTuple);
        const auto& destPort = get<1>(sailTuple);
        const auto& sailData = get<2>(sailTuple);

        // sail from the given port to some port should start before the given time (containers departs)
        bool beforeFinalDate = sailData->getStartDate()->toMinutes() <= date->toMinutes();

        if (srcPort->getName() == port->getName() && beforeFinalDate) {
            outboundContainers.emplace_back(destPort, sailData->getContainers());
        }
    }

    return outboundContainers;
}

int ContainersGraph::balance(const std::string &portName, const shared_ptr<Date> &date) {
    auto port = findPortByName(portName);
    if (port == nullptr) {
        throw PortNotExistsException(portName + " does not exists in database\n");
    }

    // get all inbound and outbound containers occurred before the given date
    auto inbound = inboundContainers(port, date);
    auto outbound = outboundContainers(port, date);

    // calculate the containers balance
    int inboundBalance = 0;
    int outboundBalance = 0;

    for (const auto& sail : inbound) {
        const auto& containers = get<1>(sail);
        inboundBalance += containers;
    }

    for (const auto& sail : outbound) {
        const auto& containers = get<1>(sail);
        outboundBalance += containers;
    }

    return inboundBalance - outboundBalance;

}

void ContainersGraph::print(std::ofstream &outStream) const {
    // header
    outStream << "Containers Graph Representation" << std::endl;

    // sails
    for (const auto& sailTuple : sails) {
        const auto& srcPort = get<0>(sailTuple);
        const auto& destPort = get<1>(sailTuple);
        const auto& sailData = get<2>(sailTuple);

        outStream << "* Sail from: " << srcPort->getName() << " (" << sailData->getStartDate()->getAsString() << ") to "
                  << destPort->getName() << " (" << sailData->getEndDate()->getAsString() << ") with "
                  <<sailData->getContainers() << " containers" << std::endl;
    }

    // separate break line
    outStream << std::endl;
}