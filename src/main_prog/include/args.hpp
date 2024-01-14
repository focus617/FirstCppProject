#pragma once

#include "app/argsparser.hpp"

enum LogLevel
{
    Fatal = 0,
    Error,
    Warn,
    Info,
    Debug,
    Trace
};

class CLI_Parser : public xuzy::ArgsParser
{
public:
    bool parse_commandline(int argc, char *argv[]) override;
};
