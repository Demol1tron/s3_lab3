#ifndef STORAGE_H
#define STORAGE_H

#include "structures.h"

struct Storage {
  NamedStructure* structures = nullptr;
  int count = 0;
};

NamedStructure* storageFind(Storage* storage, const std::string& name);
OperationRes storageAdd(Storage* storage, const std::string& name,
                        StructType type, void* structure);
OperationRes storageRemove(Storage* storage, const std::string& name);
void storageCleanup(Storage* storage);

#endif