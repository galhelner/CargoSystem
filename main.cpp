#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include "ContainersGraph.h"
#include "TimesGraph.h"
#include "PortNotExistsException.h"
#include "Commands.h"
#include "SystemUtils.h"
using namespace std;

// program's constants declarations
const string WELCOME_PROMPT = "Welcome to Boogie-lever transportation system!";
const string SUCCESSFUL_UPDATE = "Update was successful.";
const string COMMAND_PROMPT = "Please enter command: ";
const string DEFAULT_OUTPUT_FILENAME = "output.dat";
const string OUTPUT_FILE_FLAG = "-o";
const string INPUT_FILE_FLAG = "-i";

/**
 * Remove leading and trailing whitespace from a given string.
 * @param str - string with leading or trailing whitespaces.
 * @return New string without leading and trailing whitespaces.
 */
string trim(const string& str) {
    auto start = find_if_not(str.begin(), str.end(), [](unsigned char ch) {
        return isspace(ch);
    });
    auto end = find_if_not(str.rbegin(), str.rend(), [](unsigned char ch) {
       return isspace(ch);
    }).base();
    return (start < end) ? string(start, end) : "";
}

/**
 * Split command input from the user into seperated tokens.
 * @param command - command input string.
 * @param delimiter - delimiter to split by.
 * @return vector that contains all the seperated tokens.
 */
vector<string> splitCommand(const string& command, char delimiter) {
    istringstream ss(command);
    string token;
    vector<string> tokens;

    while (getline(ss, token, delimiter)) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }

    return tokens;
}

/**
 * Load transportation data from input file into the system graphs.
 * @param argc - program's arguments count.
 * @param argv - program's arguments.
 * @param containersGraph - transportation containers graph.
 * @param timesGraph - transportation times graph.
 * @return The name of the system's output file.
 */
string loadInputFiles(int argc, char* argv[], shared_ptr<ContainersGraph> containersGraph, shared_ptr<TimesGraph> timesGraph) {
    if (argv[1] != INPUT_FILE_FLAG) {
        cerr << "Usage: " << argv[0] << " -i <input_file1> [input_file2 ...] [-o] <outfile>" << endl;
    }

    string outputFilename = DEFAULT_OUTPUT_FILENAME;
    // load input files data
    for (int i = 2; i < argc; i++) {
        if (i > 2 && argv[i] == OUTPUT_FILE_FLAG && i < argc - 1) {
            outputFilename = argv[i + 1];
            break;
        }
        string inputFilename = argv[i];
        SystemUtils::loadFileToGraphs(inputFilename, containersGraph, timesGraph);
    }

    return outputFilename;
}

/**
 * Start the interaction with the user through the command line.
 * @param containersGraph - transportation containers graph.
 * @param timesGraph - transportation times graph.
 * @param outputFilename - the name of the system's output file.
 */
void runCommandLine(shared_ptr<ContainersGraph> containersGraph, shared_ptr<TimesGraph> timesGraph, const string& outputFilename) {
    // print welcome prompt
    cout << WELCOME_PROMPT << endl;

    while (true) {
        // print command prompt
        cout << COMMAND_PROMPT << endl;
        string command;
        getline(cin, command);

        // handle empty command
        if (command == "") {
            cerr << Commands::USAGE_STRING << endl;
            continue;
        }

        // exit command
        if (command == Commands::EXIT_COMMAND) {
            break;
        }

        // print command
        if (command == Commands::PRINT_COMMAND) {
            SystemUtils::printGraphs(outputFilename, containersGraph, timesGraph);
            continue;
        }

        // load command
        if (command.find(',') == std::string::npos) {
            vector<string> tokens = splitCommand(command, ' ');
            if (tokens.size() != 2 || tokens[0] != Commands::LOAD_COMMAND) {
                cerr << Commands::USAGE_STRING << endl;
                continue;
            }
            string filename = tokens[1];

            bool success = SystemUtils::loadFileToGraphs(filename, containersGraph, timesGraph);
            if (success) {
                cout << SUCCESSFUL_UPDATE << endl;
            }
            continue;
        }

        // inbound, outbound, balance commands
        vector<string> tokens = splitCommand(command, ',');
        if (tokens.size() != 2 && tokens.size() != 3) {
            cerr << Commands::USAGE_STRING << endl;
            continue;
        }
        string portName = trim(tokens[0]);
        string commandKeyword = trim(tokens[1]);
        vector<string> args;
        args.push_back(portName);
        if (tokens.size() == 3) {
            string date = trim(tokens[2]);
           args.push_back(date);
        }
        SystemUtils::executeCommand(commandKeyword, args, containersGraph, timesGraph);
    }
}

int main(int argc, char* argv[]) {
    // arguments validation
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " -i <input_file1> [input_file2 ...] [-o] <outfile>" << endl;
        return 1;
    }

    // initialize the transportation system graphs
    shared_ptr<ContainersGraph> containersGraph = make_shared<ContainersGraph>();
    shared_ptr<TimesGraph> timesGraph = make_shared<TimesGraph>();

    // load initial data
    string outputFilename = loadInputFiles(argc, argv, containersGraph, timesGraph);

    // start user interaction
    runCommandLine(containersGraph, timesGraph, outputFilename);

    return 0;
}
