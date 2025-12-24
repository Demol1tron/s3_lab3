#ifndef PARSER_H
#define PARSER_H

#include "structures.h"
#include "storage.h"
#include <vector>

struct Storage;

struct ParsedCommand {
    std::string operation;
    std::string structure_name;
    std::vector<std::string> arguments;
    StructType structure_type = StructType::UNKNOWN;
};

ParsedCommand parseQuery(const std::string &query);
OperationRes executeCommand(Storage *storage, const ParsedCommand &cmd, std::string &result);

#endif