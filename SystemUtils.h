#ifndef HW2_SYSTEMUTILS_H
#define HW2_SYSTEMUTILS_H
#include <iostream>
#include <memory>
#include "ContainersGraph.h"
#include "TimesGraph.h"
using namespace std;

/**
 * A class that provides manipulation functions on the boogie-lever transportation system.
 */
class SystemUtils {
private:
    /**
     * Check if some port name is valid.
     * @param portName - the name of the port.
     * @return True if the name is valid, otherwise false.
     */
    static bool isValidPortName(const string& portName);

    /**
     * Parse an input file into tuples represent each file's row.
     * @param inputFile - the input file to parse.
     * @return vector of tuples each represent a row in the file (Port, startDate, endDate, containers)
     */
    static vector<tuple<shared_ptr<Port>, shared_ptr<Date>, shared_ptr<Date>, int>>
    parseInputFile(const string& inputFilename);

    /**
     * Print the inbound of some port from the containers graph.
     * @param portName - the name of the port.
     * @param timesGraph - transportation times graph.
     */
    static void inbound(const string& portName, shared_ptr<TimesGraph> timesGraph);

    /**
     * Print the outbound of some port from the containers graph.
     * @param portName - the name of the port.
     * @param timesGraph - transportation times graph.
     */
    static void outbound(const string& portName, shared_ptr<TimesGraph> timesGraph);

    /**
     * Print the balance value of some port for specific date from the containers graph.
     * @param portName - the name of the port.
     * @param date - the date for the balance value calculation.
     * @param containersGraph - transportation containers graph.
     */
    static void balance(const string& portName, shared_ptr<Date> date, shared_ptr<ContainersGraph> containersGraph);
public:
    /**
     * Load sails data from an input file into the transportation graphs.
     * @param inputFilename - the name of the input file to load its data.
     * @param containersGraph - transportation containers graph.
     * @param timesGraph - transportation times graph.
     * @throws FileErrorException - if the input file not found or failed to open / read.
     * @throws InvalidInputException - if the input file is in invalid format.
     * @return True if loaded the file successfully, otherwise false.
     */
    static bool loadFileToGraphs(const string& inputFilename, shared_ptr<ContainersGraph> containersGraph, shared_ptr<TimesGraph> timesGraph);

    /**
     * Execute transportation system command.
     * @param command - command keyword.
     * @param args - command arguments.
     * @param containersGraph - transportation containers graph.
     */
    static void executeCommand(const string& command, vector<string> args, shared_ptr<ContainersGraph> containersGraph, shared_ptr<TimesGraph> timesGraph);

    /**
     * Print the representation of the containers and times graphs into the output file.
     * @param outputFilename - the name of the output file.
     */
    static void printGraphs(const string& outputFilename, shared_ptr<ContainersGraph> containersGraph, shared_ptr<TimesGraph> timesGraph);
};


#endif //HW2_SYSTEMUTILS_H
