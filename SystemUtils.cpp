#include "SystemUtils.h"
#include "FileErrorException.h"
#include "InvalidInputException.h"
#include "DateFormatException.h"
#include "PortNotExistsException.h"
#include "Commands.h"
#include <fstream>
#include <sstream>
#include <string>

bool SystemUtils::isValidPortName(const std::string &portName) {
    // length validation
    if (portName.size() > 16) {
        return false;
    }

    // alpha or space characters only validation
    for (char ch : portName) {
        if (!isalpha(ch) && !isspace(ch)) {
            return false;
        }
    }

    return true;
}

vector<tuple<shared_ptr<Port>, shared_ptr<Date>, shared_ptr<Date>, int>>
SystemUtils::parseInputFile(const string & inputFilename) {
    vector<tuple<shared_ptr<Port>, shared_ptr<Date>, shared_ptr<Date>, int>> parsedContent;
    string line;
    int lineNum = 0;

    ifstream inputFile(inputFilename);
    if (!inputFile) {
        throw FileErrorException(" ERROR opening/reading the specified file.");
    }

    while (getline(inputFile, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        istringstream ss(line);
        string token;
        vector<string> tokens;
        string lineNumStr = to_string(lineNum + 1);

        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        if (lineNum == 0) {
            // first line of the file
            if (tokens.size() == 2) {
                string portName = tokens[0];
                string startTimeStr = tokens[1];
                if (!isValidPortName(portName)) {
                    throw InvalidInputException("Invalid input in file " + inputFilename + " at line " + lineNumStr + "\n");
                }
                try {
                    shared_ptr<Date> startTime = make_shared<Date>(startTimeStr);
                    shared_ptr<Port> port = make_shared<Port>(portName);
                    parsedContent.emplace_back(port, startTime, nullptr, 0);
                } catch (DateFormatException&) {
                    throw InvalidInputException("Invalid input in file " + inputFilename + " at line " + lineNumStr + "\n");
                }
            } else {
                // invalid length for the first line
                throw InvalidInputException("Invalid input in file " + inputFilename + " at line " + lineNumStr + "\n");
            }
        } else {
            if (tokens.size() == 4) {
                string portName = tokens[0];
                string startTimeStr = tokens[1];
                string containersStr = tokens[2];
                string endTimeStr = tokens[3];

                if (!isValidPortName(portName)) {
                    throw InvalidInputException("Invalid input in file " + inputFilename + " at line " + lineNumStr + "\n");
                }

                try {
                    int containers = stoi(containersStr);

                    if (containers < 0) {
                        throw InvalidInputException("Invalid input in file " + inputFilename + " at line " + lineNumStr + "\n");
                    }

                    shared_ptr<Date> startTime = make_shared<Date>(startTimeStr);
                    shared_ptr<Date> endTime = make_shared<Date>(endTimeStr);
                    shared_ptr<Port> port = make_shared<Port>(portName);
                    parsedContent.emplace_back(port, startTime, endTime, containers);
                } catch (std::invalid_argument&) {
                    throw InvalidInputException("Invalid input in file " + inputFilename + " at line " + lineNumStr + "\n");
                } catch (DateFormatException&) {
                    throw InvalidInputException("Invalid input in file " + inputFilename + " at line " + lineNumStr + "\n");
                }

            } else {
                // invalid length for any other line
                throw InvalidInputException("Invalid input in file " + inputFilename + " at line " + lineNumStr + "\n");
            }
        }

        lineNum++;
    }

    return parsedContent;
}

bool SystemUtils::loadFileToGraphs(const std::string &inputFilename, shared_ptr<ContainersGraph> containersGraph,
                                   shared_ptr<TimesGraph> timesGraph) {
    try {
        auto parsedContent = parseInputFile(inputFilename);
        auto firstTuple = parsedContent[0];
        auto firstPort = get<0>(firstTuple);
        auto firstTime = get<1>(firstTuple);

        // load data to the graphs
        for (size_t i = 0; i < parsedContent.size(); i++) {
            auto tuple = parsedContent[i];
            auto port = get<0>(tuple);
            auto startTime = get<1>(tuple);
            auto endTime = get<2>(tuple);
            auto containers = get<3>(tuple);

            // containers graph
            containersGraph->addPort(port);
            if (i > 0) {
                containersGraph->addSail(firstPort, port, firstTime, startTime, containers);
            }

            // times graph
            timesGraph->addPort(port);
            if (i < parsedContent.size() - 1) {
                auto nextTuple = parsedContent[i + 1];
                auto nextPort = get<0>(nextTuple);
                auto nextStartTime = get<1>(nextTuple);
                if (i == 0) {
                    timesGraph->addSail(port, nextPort, startTime, nextStartTime);
                } else {
                    timesGraph->addSail(port, nextPort, endTime, nextStartTime);
                }
            }
        }
        return true;

    } catch (FileErrorException& e) {
        cerr << e.what() << endl;
        return false;
    } catch (InvalidInputException& e) {
        cerr << e.what() << endl;
        return false;
    }
}

void SystemUtils::inbound(const std::string &portName, shared_ptr<TimesGraph> timesGraph) {
    try {
        auto inboundPorts = timesGraph->inbound(portName);
        if (inboundPorts.empty()) {
            cout << portName << ": " << "no inbound ports" << endl;
            return;
        }
        for (const auto& inboundTuple : inboundPorts) {
            const auto& port = get<0>(inboundTuple);
            const auto& time = get<1>(inboundTuple);
            cout << port->getName() << ", " << time << endl;
        }
    } catch (PortNotExistsException& e) {
        cerr << e.what() << endl;
    }
}

void SystemUtils::outbound(const std::string &portName, shared_ptr<TimesGraph> timesGraph) {
    try {
        auto outboundPorts = timesGraph->outbound(portName);
        if (outboundPorts.empty()) {
            cout << portName << ": " << "no outbound ports" << endl;
            return;
        }
        for (const auto& outboundTuple : outboundPorts) {
            const auto& port = get<0>(outboundTuple);
            const auto& time = get<1>(outboundTuple);
            cout << port->getName() << ", " << time << endl;
        }
    } catch (PortNotExistsException& e) {
        cerr << e.what() << endl;
    }
}

void
SystemUtils::balance(const std::string &portName, shared_ptr<Date> date, shared_ptr<ContainersGraph> containersGraph) {
    try {
        int balanceValue = containersGraph->balance(portName, date);
        cout << balanceValue << endl;
    } catch (PortNotExistsException& e) {
        cerr << e.what() << endl;
    }
}

void SystemUtils::executeCommand(const std::string &command, vector<string> args, shared_ptr<ContainersGraph> containersGraph, shared_ptr<TimesGraph> timesGraph) {
    string portName = args[0];
    if (command == Commands::INBOUND_COMMAND) {
        if (args.size() != 1) {
            cerr << Commands::USAGE_STRING << endl;
            return;
        }
        inbound(portName, timesGraph);
    } else if (command == Commands::OUTBOUND_COMMAND) {
        if (args.size() != 1) {
            cerr << Commands::USAGE_STRING << endl;
            return;
        }
        outbound(portName, timesGraph);
    } else if (command == Commands::BALANCE_COMMAND) {
        if (args.size() != 2) {
            cerr << Commands::USAGE_STRING << endl;
            return;
        }
        string dateStr = args[1];
        try {
            shared_ptr<Date> date = make_shared<Date>(dateStr);
            balance(portName, date, containersGraph);
        } catch (DateFormatException& e) {
            cerr << Commands::USAGE_STRING << endl;
            return;
        }
    } else {
        // invalid command
        cerr << Commands::USAGE_STRING << endl;
        return;
    }
}

void SystemUtils::printGraphs(const std::string &outputFilename, shared_ptr<ContainersGraph> containersGraph, shared_ptr<TimesGraph> timesGraph) {
    ofstream outFile(outputFilename);

    if (!outFile) {
        cerr << "Failed to open output file: " << outputFilename << endl;
        return;
    }

    containersGraph->print(outFile);
    timesGraph->print(outFile);
}
