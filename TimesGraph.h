#ifndef HW2_TIMESGRAPH_H
#define HW2_TIMESGRAPH_H
#include <iostream>
#include <memory>
#include <vector>
#include <set>
#include "Port.h"
#include "SailTimesData.h"
#include "Date.h"
using namespace std;

/**
 * A class represent the times graph
 */
class TimesGraph {
private:
    // vertexes
    set<shared_ptr<Port>> ports;
    // edges
    vector<tuple<shared_ptr<Port>, shared_ptr<Port>, shared_ptr<SailTimesData>>> sails;

    /**
     * Check if a sail (graph's edge) is already exists
     * @param srcPort - sail's source port
     * @param destPort - sail's destination port
     * @return tuple of the sail if found, otherwise nullptr.
     */
    tuple<shared_ptr<Port>, shared_ptr<Port>, shared_ptr<SailTimesData>>
    sailExists(shared_ptr<Port> srcPort, shared_ptr<Port> destPort);

    /**
     * Find a port vertex by name.
     * @param portName - the name of the port.
     * @return Shared pointer to the found port object or nullptr if not found.
     */
    shared_ptr<Port> findPortByName(const string& portName);
public:
    /**
     * Add a new port as vertex to the times graph.
     * @param port - port to add
     */
    void addPort(shared_ptr<Port> port);

    /**
    * Add a new sail as an edge to the times graph.
    * @param srcPort - sail's source port
    * @param destPort - sail's destination port
    * @param startDate - starting date of the sail
    * @param endDate - ending date of the sail
    */
    void addSail(shared_ptr<Port> srcPort, shared_ptr<Port> destPort, shared_ptr<Date> startDate, shared_ptr<Date> endDate);

    /**
     * Find all the ports that are reachable in a single edge from given port.
     * @param portName - the name of the port.
     * @throws PortNotExistsException - if there's no port with the given name.
     * @return vector of tuples which contains the reachable ports and their sail time.
     */
    vector<tuple<shared_ptr<Port>, int>> outbound(const string& portName);

    /**
     * Find all the ports that the given port is reachable from them.
     * @param portName - the name of the port.
     * @throws PortNotExistsException - if there's no port with the given name.
     * @return vector of tuples which contains the ports can reach the given port and their sail time.
     */
    vector<tuple<shared_ptr<Port>, int>> inbound(const string& portName);

    /**
     * Print representation of the times graph into a given stream.
     * @param outStream - reference for the output file stream
     */
    void print(ofstream& outStream) const;
};


#endif //HW2_TIMESGRAPH_H
