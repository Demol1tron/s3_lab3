#include "storage.h"

auto storageFind(Storage* storage, const std::string& name) -> NamedStructure* {
  if (storage == nullptr) {
    return nullptr;
  }

  NamedStructure* current = storage->structures;
  while (current != nullptr) {
    if (current->name == name) {
      return current;
    }
    current = current->next;
  }
  return nullptr;
}

auto storageAdd(Storage* storage, const std::string& name, StructType type,
                void* structure) -> OperationRes {
  if (storage == nullptr) {
    return OperationRes::ERROR;
  }

  // уникальность имени
  if (storageFind(storage, name) != nullptr) {
    return OperationRes::ERROR;
  }

  auto* newNode = new NamedStructure(name, type, structure);

  // в начало списка
  newNode->next = storage->structures;
  storage->structures = newNode;
  storage->count++;

  return OperationRes::SUCCESS;
}

auto storageRemove(Storage* storage, const std::string& name) -> OperationRes {
  if ((storage == nullptr) || (storage->structures == nullptr)) {
    return OperationRes::NOT_FOUND;
  }

  NamedStructure* current = storage->structures;
  NamedStructure* prev = nullptr;

  while (current != nullptr) {
    if (current->name == name) {
      if (prev != nullptr) {
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
  if (storage == nullptr) {
    return;
  }

  NamedStructure* current = storage->structures;
  while (current != nullptr) {
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