#include <fstream>
#include "TimesGraph.h"
#include "PortNotExistsException.h"

tuple<shared_ptr<Port>, shared_ptr<Port>, shared_ptr<SailTimesData>>
TimesGraph::sailExists(shared_ptr<Port> srcPort, shared_ptr<Port> destPort) {
    for (const auto& sailTuple : sails) {
        const auto& foundedSrcPort = get<0>(sailTuple);
        const auto& foundedDestPort = get<1>(sailTuple);

        bool sameSrc = foundedSrcPort->getName() == srcPort->getName();
        bool sameDest = foundedDestPort->getName() == destPort->getName();

        // found edge with the same values
        if (sameSrc && sameDest) {
            return sailTuple;
        }
    }

    // no matching edge found
    return make_tuple(nullptr, nullptr, nullptr);
}

shared_ptr<Port> TimesGraph::findPortByName(const std::string &portName) {
    for (const auto& port : ports) {
        if (port->getName() == portName) {
            return port;
        }
    }
    return nullptr;
}

void TimesGraph::addPort(shared_ptr<Port> port) {
    this->ports.insert(std::move(port));
}

void TimesGraph::addSail(shared_ptr<Port> srcPort, shared_ptr<Port> destPort, shared_ptr<Date> startDate,
                         shared_ptr<Date> endDate) {
    int sailTime = endDate->minutesSince(startDate);
    int newSailTime;

    auto sailExistsTuple = sailExists(srcPort, destPort);
    const auto& foundedSrcPort = get<0>(sailExistsTuple);
    const auto& foundedDestPort = get<1>(sailExistsTuple);
    const auto& foundedSailData = get<2>(sailExistsTuple);

    if (!foundedSrcPort && !foundedDestPort && !foundedSailData) {
        // the new edge does not exist in the graph
        shared_ptr<SailTimesData> sailData = make_shared<SailTimesData>(startDate, endDate, sailTime);
        this->sails.emplace_back(std::move(srcPort), std::move(destPort), sailData);
    } else {
        // edge already exists, update the average sail time
        newSailTime = (foundedSailData->getAverageSailTime() + sailTime) / 2;
        foundedSailData->setAverageSailTime(newSailTime);
    }
}

vector<tuple<shared_ptr<Port>, int>> TimesGraph::outbound(const std::string &portName) {
    vector<tuple<shared_ptr<Port>, int>> outboundPorts;
    auto port = findPortByName(portName);
    if (port == nullptr) {
        throw PortNotExistsException(portName + " does not exist in the database.\n");
    }

    for (const auto& sailTuple : sails) {
        const auto& srcPort = get<0>(sailTuple);
        const auto& destPort = get<1>(sailTuple);
        const auto& sailData = get<2>(sailTuple);

        if (srcPort->getName() == port->getName()) {
            int sailTime = sailData->getAverageSailTime();
            outboundPorts.emplace_back(destPort, sailTime);
        }
    }

    return outboundPorts;
}

vector<tuple<shared_ptr<Port>, int>> TimesGraph::inbound(const std::string &portName) {
    vector<tuple<shared_ptr<Port>, int>> inboundPorts;
    auto port = findPortByName(portName);
    if (port == nullptr) {
        throw PortNotExistsException(portName + " does not exist in the database.\n");
    }

    for (const auto& sailTuple : sails) {
        const auto& srcPort = get<0>(sailTuple);
        const auto& destPort = get<1>(sailTuple);
        const auto& sailData = get<2>(sailTuple);

        if (destPort->getName() == port->getName()) {
            int sailTime = sailData->getEndDate()->minutesSince(sailData->getStartDate());
            inboundPorts.emplace_back(srcPort, sailTime);
        }
    }

    return inboundPorts;
}

void TimesGraph::print(std::ofstream &outStream) const {
    // header
    outStream << "Times Graph Representation" << std::endl;

    // sails
    for (const auto& sailTuple : sails) {
        const auto& srcPort = get<0>(sailTuple);
        const auto& destPort = get<1>(sailTuple);
        const auto& sailData = get<2>(sailTuple);

        outStream << "* Sail from: " << srcPort->getName() << " to " << destPort->getName() << " within " << sailData->getAverageSailTime() << " minutes" << endl;
    }

    // separate break line
    outStream << std::endl;
}
