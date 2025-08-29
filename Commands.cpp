#include "Commands.h"

const string Commands::LOAD_COMMAND = "load";
const string Commands::OUTBOUND_COMMAND = "outbound";
const string Commands::INBOUND_COMMAND = "inbound";
const string Commands::BALANCE_COMMAND = "balance";
const string Commands::PRINT_COMMAND = "print";
const string Commands::EXIT_COMMAND = "exit";
const string Commands::USAGE_STRING = "USAGE:\t‘load’ <file> *or* \n\t\t"
                                      " <node>,'inbound' *or* \n\t\t"
                                      " <node>,’outbound’ *or* \n\t\t"
                                      " <node>,’balance’,dd/mm HH:mm *or* \n\t\t"
                                      " ‘print’ *or* \n\t\t ‘exit’ *to terminate*";