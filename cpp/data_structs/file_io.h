#ifndef FILE_IO_H
#define FILE_IO_H

#include <string>

#include "storage.h"
#include "structures.h"

OperationRes saveToFile(const Storage* storage, const std::string& filename);
OperationRes loadFromFile(Storage* storage, const std::string& filename);

#endif
