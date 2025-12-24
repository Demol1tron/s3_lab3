#include "parser.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

#include "storage.h"
#include "structures.h"

auto toLower(std::string s) -> std::string {
  std::transform(s.begin(), s.end(), s.begin(),
                 [](unsigned char c) -> int { return std::tolower(c); });
  return s;
}

auto parseQuery(const std::string& query) -> ParsedCommand {
  ParsedCommand cmd;
  std::istringstream iss(query);
  std::string token;

  // первое слово
  if (iss >> token) {
    cmd.operation = token;
    // std::transform(cmd.operation.begin(), cmd.operation.end(),
    // cmd.operation.begin(), ::toupper);
  }

  // второе слово
  if (iss >> token) {
    cmd.structure_name = token;
  }

  while (iss >> token) {
    cmd.arguments.push_back(token);
  }

  return cmd;
}

auto executeCommand(Storage* storage, const ParsedCommand& cmd,
                    std::string& result) -> OperationRes {
  if (storage == nullptr) {
    return OperationRes::ERROR;
  }

  if (cmd.operation == "CREATE") {
    if (cmd.arguments.empty()) {
      return OperationRes::ERROR;
    }

    std::string typeStr = cmd.arguments[0];
    void* newStruct = nullptr;
    StructType typeEnum = StructType::UNKNOWN;

    if (typeStr == "ARRAY") {
      newStruct = new M(cmd.structure_name);
      typeEnum = StructType::ARRAY;
    } else if (typeStr == "SINGLY_LIST" || typeStr == "LIST") {
      newStruct = new F(cmd.structure_name);
      typeEnum = StructType::SINGLY_LIST;
    } else if (typeStr == "DOUBLY_LIST" || typeStr == "DLIST") {
      newStruct = new L(cmd.structure_name);
      typeEnum = StructType::DOUBLY_LIST;
    } else if (typeStr == "STACK") {
      newStruct = new S(cmd.structure_name);
      typeEnum = StructType::STACK;
    } else if (typeStr == "QUEUE") {
      newStruct = new Q(cmd.structure_name);
      typeEnum = StructType::QUEUE;
    } else if (typeStr == "TREE") {
      newStruct = new T(cmd.structure_name);
      typeEnum = StructType::TREE;
    } else if (typeStr == "SET") {
      newStruct = new Set(cmd.structure_name);
      typeEnum = StructType::SET;
    } else {
      return OperationRes::ERROR;
    }

    return storageAdd(storage, cmd.structure_name, typeEnum, newStruct);
  }

  if (cmd.operation == "PRINT") {
    if (cmd.structure_name.empty() || cmd.structure_name == "ALL") {
      NamedStructure* current = storage->structures;
      if (current == nullptr) {
        std::cout << "Хранилище пусто." << '\n';
        return OperationRes::SUCCESS;
      }
      while (current != nullptr) {
        switch (current->type) {
          case StructType::ARRAY:
            static_cast<M*>(current->structure)->print();
            break;
          case StructType::SINGLY_LIST:
            static_cast<F*>(current->structure)->print();
            break;
          case StructType::DOUBLY_LIST:
            static_cast<L*>(current->structure)->printForward();
            break;
          case StructType::STACK:
            static_cast<S*>(current->structure)->print();
            break;
          case StructType::QUEUE:
            static_cast<Q*>(current->structure)->print();
            break;
          case StructType::TREE:
            static_cast<T*>(current->structure)->print();
            break;
          case StructType::SET:
            static_cast<Set*>(current->structure)->print();
            break;
          default:
            std::cout << "Неизвестный тип структуры" << '\n';
            break;
        }
        std::cout << '\n';
        current = current->next;
      }
      return OperationRes::SUCCESS;
    }
    NamedStructure* namedStruct = storageFind(storage, cmd.structure_name);
    if (namedStruct == nullptr) {
      return OperationRes::NOT_FOUND;
    }

    switch (namedStruct->type) {
      case StructType::ARRAY:
        static_cast<M*>(namedStruct->structure)->print();
        break;
      case StructType::SINGLY_LIST:
        static_cast<F*>(namedStruct->structure)->print();
        break;
      case StructType::DOUBLY_LIST:
        static_cast<L*>(namedStruct->structure)->printForward();
        break;
      case StructType::STACK:
        static_cast<S*>(namedStruct->structure)->print();
        break;
      case StructType::QUEUE:
        static_cast<Q*>(namedStruct->structure)->print();
        break;
      case StructType::TREE:
        static_cast<T*>(namedStruct->structure)->print();
        break;
      case StructType::SET:
        static_cast<Set*>(namedStruct->structure)->print();
        break;
      default:
        return OperationRes::ERROR;
    }
    return OperationRes::SUCCESS;
  }

  NamedStructure* namedStruct = storageFind(storage, cmd.structure_name);
  if (namedStruct == nullptr) {
    if (cmd.operation == "DELETE" || cmd.operation == "EXISTS") {
      //
    } else {
      return OperationRes::NOT_FOUND;
    }
  }

  if ((namedStruct != nullptr) && namedStruct->type == StructType::ARRAY) {
    M* arr = static_cast<M*>(namedStruct->structure);
    if (cmd.operation == "MPUSH" && !cmd.arguments.empty()) {
      if (cmd.arguments.size() == 1) {
        return arr->pushBack(cmd.arguments[0]);
      }
      size_t index = std::stoul(cmd.arguments[0]);
      return arr->pushAt(index, cmd.arguments[1]);
    }
    if (cmd.operation == "MGET" && cmd.arguments.size() == 1) {
      size_t index = std::stoul(cmd.arguments[0]);
      return arr->get(index, result);
    } if (cmd.operation == "MDEL" && cmd.arguments.size() == 1) {
      size_t index = std::stoul(cmd.arguments[0]);
      return arr->delAt(index);
    } else if (cmd.operation == "MREPLACE" && cmd.arguments.size() == 2) {
      size_t index = std::stoul(cmd.arguments[0]);
      return arr->replace(index, cmd.arguments[1]);
    } else if (cmd.operation == "MFIND" && cmd.arguments.size() == 1) {
      int index = arr->find(cmd.arguments[0]);
      if (index != -1) {
        result = std::to_string(index);
        return OperationRes::SUCCESS;
      }
      return OperationRes::NOT_FOUND;
    } else if (cmd.operation == "MSIZE") {
      result = std::to_string(arr->getSize());
      return OperationRes::SUCCESS;
    }
  }

  else if ((namedStruct != nullptr) &&
           namedStruct->type == StructType::SINGLY_LIST) {
    F* list = static_cast<F*>(namedStruct->structure);
    if (cmd.operation == "FPUSH" && !cmd.arguments.empty()) {
      if (cmd.arguments.size() == 1) {
        return list->pushBack(cmd.arguments[0]);
      }
      if (cmd.arguments.size() == 2) {
        std::string position = toLower(cmd.arguments[0]);
        if (position == "front") {
          return list->pushFront(cmd.arguments[1]);
        }
        if (position == "back") { return list->pushBack(cmd.arguments[1]);
}
      }
    } else if (cmd.operation == "FPUSHAFTER" && cmd.arguments.size() == 2) {
      FNode* target = list->find(cmd.arguments[0]);
      if (target == nullptr) {
        return OperationRes::NOT_FOUND;
      }
      return list->pushAfter(target, cmd.arguments[1]);
    } else if (cmd.operation == "FPUSHBEFORE" && cmd.arguments.size() == 2) {
      FNode* target = list->find(cmd.arguments[0]);
      if (target == nullptr) {
        return OperationRes::NOT_FOUND;
      }
      return list->pushBefore(target, cmd.arguments[1]);
    } else if (cmd.operation == "FDEL" && !cmd.arguments.empty()) {
      std::string position = toLower(cmd.arguments[0]);
      if (position == "front") {
        return list->delFront();
      }
      if (position == "back") {
        return list->delBack();
      }
      if (position == "value" && cmd.arguments.size() == 2) {
        return list->delByValue(cmd.arguments[1]);
}
    } else if (cmd.operation == "FDELAFTER" && cmd.arguments.size() == 1) {
      FNode* target = list->find(cmd.arguments[0]);
      if (target == nullptr) {
        return OperationRes::NOT_FOUND;
      }
      return list->delAfter(target);
    } else if (cmd.operation == "FDELBEFORE" && cmd.arguments.size() == 1) {
      FNode* target = list->find(cmd.arguments[0]);
      if (target == nullptr) {
        return OperationRes::NOT_FOUND;
      }
      return list->delBefore(target);
    } else if (cmd.operation == "FGET" && cmd.arguments.size() == 1) {
      if (cmd.arguments[0] == "size") {
        result = std::to_string(list->getLength());
        return OperationRes::SUCCESS;
      }
    } else if (cmd.operation == "FFIND" && cmd.arguments.size() == 1) {
      if (list->find(cmd.arguments[0]) != nullptr) {
        result = "НАЙДЕН";
        return OperationRes::SUCCESS;
      }
      return OperationRes::NOT_FOUND;
    }
  }

  else if ((namedStruct != nullptr) &&
           namedStruct->type == StructType::DOUBLY_LIST) {
    L* list = static_cast<L*>(namedStruct->structure);
    if (cmd.operation == "LPUSH" && !cmd.arguments.empty()) {
      if (cmd.arguments.size() == 1) {
        return list->pushBack(cmd.arguments[0]);
      }
      if (cmd.arguments.size() == 2) {
        std::string position = toLower(cmd.arguments[0]);
        if (position == "front") {
          return list->pushFront(cmd.arguments[1]);
        }
        if (position == "back") { return list->pushBack(cmd.arguments[1]);
}
      }
    } else if (cmd.operation == "LPUSHAFTER" && cmd.arguments.size() == 2) {
      LNode* target = list->find(cmd.arguments[0]);
      if (target == nullptr) {
        return OperationRes::NOT_FOUND;
      }
      return list->pushAfter(target, cmd.arguments[1]);
    } else if (cmd.operation == "LPUSHBEFORE" && cmd.arguments.size() == 2) {
      LNode* target = list->find(cmd.arguments[0]);
      if (target == nullptr) {
        return OperationRes::NOT_FOUND;
      }
      return list->pushBefore(target, cmd.arguments[1]);
    } else if (cmd.operation == "LDELAFTER" && cmd.arguments.size() == 1) {
      LNode* target = list->find(cmd.arguments[0]);
      if (target == nullptr) {
        return OperationRes::NOT_FOUND;
      }
      return list->delAfter(target);
    } else if (cmd.operation == "LDELBEFORE" && cmd.arguments.size() == 1) {
      LNode* target = list->find(cmd.arguments[0]);
      if (target == nullptr) {
        return OperationRes::NOT_FOUND;
      }
      return list->delBefore(target);
    } else if (cmd.operation == "LDEL" && !cmd.arguments.empty()) {
      std::string position = toLower(cmd.arguments[0]);
      if (position == "front") {
        return list->delFront();
      }
      if (position == "back") {
        return list->delBack();
      }
      if (position == "value" && cmd.arguments.size() == 2) {
        return list->delByValue(cmd.arguments[1]);
}
    } else if (cmd.operation == "LGET" && cmd.arguments.size() == 1) {
      if (cmd.arguments[0] == "size") {
        result = std::to_string(list->getLength());
        return OperationRes::SUCCESS;
      }
    }
  }

  else if ((namedStruct != nullptr) && namedStruct->type == StructType::STACK) {
    S* stack = static_cast<S*>(namedStruct->structure);
    if (cmd.operation == "SPUSH" && cmd.arguments.size() == 1) {
      return stack->push(cmd.arguments[0]);
    }
    if (cmd.operation == "SPOP") {
      return stack->pop(result);
    }
    if (cmd.operation == "SSIZE") {
      result = std::to_string(stack->getSize());
      return OperationRes::SUCCESS;
    }
  }

  else if ((namedStruct != nullptr) && namedStruct->type == StructType::QUEUE) {
    Q* queue = static_cast<Q*>(namedStruct->structure);
    if (cmd.operation == "QPUSH" && cmd.arguments.size() == 1) {
      return queue->enqueue(cmd.arguments[0]);
    }
    if (cmd.operation == "QPOP") {
      return queue->pop(result);
    }
    if (cmd.operation == "QSIZE") {
      result = std::to_string(queue->getSize());
      return OperationRes::SUCCESS;
    }
  }

  else if ((namedStruct != nullptr) && namedStruct->type == StructType::TREE) {
    T* tree = static_cast<T*>(namedStruct->structure);
    if (cmd.operation == "TINSERT" && cmd.arguments.size() == 1) {
      return tree->insert(cmd.arguments[0]);
    }
    if (cmd.operation == "TSEARCH" && cmd.arguments.size() == 1) {
      result = (tree->search(cmd.arguments[0]) == OperationRes::SUCCESS)
                   ? "ДА"
                   : "НЕТ";
      return OperationRes::SUCCESS;
    }
    if (cmd.operation == "TISFULL") {
      result = tree->isFull() ? "ДА" : "НЕТ";
      return OperationRes::SUCCESS;
    } if (cmd.operation == "TSIZE") {
      result = std::to_string(tree->getSize());
      return OperationRes::SUCCESS;
    }
  }

  else if ((namedStruct != nullptr) && namedStruct->type == StructType::SET) {
    Set* set = static_cast<Set*>(namedStruct->structure);
    if (cmd.operation == "SETADD" && cmd.arguments.size() == 1) {
      return set->add(cmd.arguments[0]);
    }
    if (cmd.operation == "SETDEL" && cmd.arguments.size() == 1) {
      return set->remove(cmd.arguments[0]);
    }
    if (cmd.operation == "SETSIZE") {
      result = std::to_string(set->getSize());
      return OperationRes::SUCCESS;
    } if (cmd.operation == "SETAT" && cmd.arguments.size() == 1) {
      if (set->contains(cmd.arguments[0])) {
        result = "TRUE";
        return OperationRes::SUCCESS;
      }
      return OperationRes::NOT_FOUND;
    }
  }

  if (cmd.operation == "DELETE") {
    return storageRemove(storage, cmd.structure_name);
  }
  if (cmd.operation == "EXISTS") {
    result = (namedStruct != nullptr) ? "ДА" : "НЕТ";
    return OperationRes::SUCCESS;
  }
  return OperationRes::ERROR;
}