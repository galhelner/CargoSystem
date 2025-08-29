#ifndef HW2_CONTAINERSGRAPH_H
#define HW2_CONTAINERSGRAPH_H
#include <iostream>
#include <memory>
#include <vector>
#include <set>
#include <unordered_set>
#include <algorithm>
#include "Port.h"
#include "Date.h"
#include "SailContainersData.h"
using namespace std;

/**
 * A class that represent the containers graph.
 */
class ContainersGraph {
private:
    // vertexes
    set<shared_ptr<Port>> ports;
    // edges
    vector<tuple<shared_ptr<Port>, shared_ptr<Port>, shared_ptr<SailContainersData>>> sails;

    /**
     * Check if some sail (graph edge) is already exists.
     * @param srcPort - sail's source port
     * @param destPort - sail's destination port
     * @param startDate - starting date of the sail
     * @param endDate - ending date of the sail
     * @return True if the given sail already exists, otherwise false.
     */
    bool sailExists(shared_ptr<Port> srcPort, shared_ptr<Port> destPort, shared_ptr<Date> startDate, shared_ptr<Date> endDate);

    /**
     * Find a port vertex by name.
     * @param portName - the name of the port.
     * @return Shared pointer to the found port object or nullptr if not found.
     */
    shared_ptr<Port> findPortByName(const string& portName);

    /**
     * Find all the inbound sails and their containers amount that occurred before some date.
     * @param port - reference to the port.
     * @param date - the date to get the inbounds before it.
     * @return vector of tuples which contains the inbound sails and their container amount.
     */
    vector<tuple<shared_ptr<Port>, int>> inboundContainers(const shared_ptr<Port>& port, const shared_ptr<Date>& date);

    /**
    * Find all the outbound sails and their containers amount that occurred before some date.
    * @param port - reference to the port.
    * @param date - the date to get the outbounds before it.
    * @return vector of tuples which contains the outbound sails and their container amount.
    */
    vector<tuple<shared_ptr<Port>, int>> outboundContainers(const shared_ptr<Port>& port, const shared_ptr<Date>& date);
public:
    /**
     * Add a new port as vertex to the containers graph.
     * @param port - port to add
     */
    void addPort(shared_ptr<Port> port);

    /**
     * Add a new sail as an edge to the containers graph.
     * @param srcPort - sail's source port
     * @param destPort - sail's destination port
     * @param startDate - starting date of the sail
     * @param endDate - ending date of the sail
     * @param containers - amount of containers on the sail
     */
    void addSail(shared_ptr<Port> srcPort, shared_ptr<Port> destPort, shared_ptr<Date> startDate, shared_ptr<Date> endDate, int containers);

    /**
     * Get the containers balance of a given port at specific date.
     * @param date - date of the desire balance value
     * @param portName - the name of the port.
     * @throws PortNotExistsException - if there's no port with the given name.
     * @return The desired balance value of the given port at the specific date.
     */
    int balance(const string& portName, const shared_ptr<Date>& date);

    /**
     * Print representation of the containers graph into a given stream.
     * @param outStream - reference for the output file stream
     */
    void print(ofstream& outStream) const;
};


#endif //HW2_CONTAINERSGRAPH_H
