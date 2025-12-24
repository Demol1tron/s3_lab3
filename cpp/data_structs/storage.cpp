#include "storage.h"

NamedStructure* storageFind(Storage* storage, const std::string& name) {
  if (!storage) return nullptr;

  NamedStructure* current = storage->structures;
  while (current) {
    if (current->name == name) {
      return current;
    }
    current = current->next;
  }
  return nullptr;
}

OperationRes storageAdd(Storage* storage, const std::string& name,
                        StructType type, void* structure) {
  if (!storage) return OperationRes::ERROR;

  // уникальность имени
  if (storageFind(storage, name)) {
    return OperationRes::ERROR;
  }

  NamedStructure* newNode = new NamedStructure(name, type, structure);

  // в начало списка
  newNode->next = storage->structures;
  storage->structures = newNode;
  storage->count++;

  return OperationRes::SUCCESS;
}

OperationRes storageRemove(Storage* storage, const std::string& name) {
  if (!storage || !storage->structures) return OperationRes::NOT_FOUND;

  NamedStructure* current = storage->structures;
  NamedStructure* prev = nullptr;

  while (current) {
    if (current->name == name) {
      if (prev) {
        prev->next = current->next;
      } else {
        storage->structures = current->next;
      }

      switch (current->type) {
        case StructType::ARRAY:
          delete static_cast<M*>(current->structure);
          break;
        case StructType::SINGLY_LIST:
          delete static_cast<F*>(current->structure);
          break;
        case StructType::DOUBLY_LIST:
          delete static_cast<L*>(current->structure);
          break;
        case StructType::STACK:
          delete static_cast<S*>(current->structure);
          break;
        case StructType::QUEUE:
          delete static_cast<Q*>(current->structure);
          break;
        case StructType::TREE:
          delete static_cast<T*>(current->structure);
          break;
        case StructType::SET:
          delete static_cast<Set*>(current->structure);
          break;
        default:
          break;
      }

      delete current;  // удаляем узел списка NamedStructure
      storage->count--;
      return OperationRes::SUCCESS;
    }
    prev = current;
    current = current->next;
  }

  return OperationRes::NOT_FOUND;
}

void storageCleanup(Storage* storage) {
  if (!storage) return;

  NamedStructure* current = storage->structures;
  while (current) {
    NamedStructure* next = current->next;

    // Удаляем саму структуру данных
    switch (current->type) {
      case StructType::ARRAY:
        delete static_cast<M*>(current->structure);
        break;
      case StructType::SINGLY_LIST:
        delete static_cast<F*>(current->structure);
        break;
      case StructType::DOUBLY_LIST:
        delete static_cast<L*>(current->structure);
        break;
      case StructType::STACK:
        delete static_cast<S*>(current->structure);
        break;
      case StructType::QUEUE:
        delete static_cast<Q*>(current->structure);
        break;
      case StructType::TREE:
        delete static_cast<T*>(current->structure);
        break;
      case StructType::SET:
        delete static_cast<Set*>(current->structure);
        break;
      default:
        break;
    }

    delete current;
    current = next;
  }

  storage->structures = nullptr;
  storage->count = 0;
}