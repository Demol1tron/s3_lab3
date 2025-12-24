#ifndef FILE_IO_H
#define FILE_IO_H

#include "structures.h"
#include "storage.h"
#include <string>

OperationRes saveToFile(const Storage *storage, const std::string &filename);
OperationRes loadFromFile(Storage *storage, const std::string &filename);

#endif
