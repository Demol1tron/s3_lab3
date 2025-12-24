#include "file_io.h"

#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <vector>

OperationRes saveToFile(const Storage* storage, const std::string& filename) {
  if (!storage) return OperationRes::ERROR;

  std::ofstream file(filename);
  if (!file.is_open()) return OperationRes::ERROR;

  file << "#ДБ\n#ТИП/ИМЯ/РАЗМЕР/ДАННЫЕ" << std::endl;

  NamedStructure* current = storage->structures;
  while (current) {
    file << static_cast<int>(current->type) << " " << current->name << " ";

    switch (current->type) {
      case StructType::ARRAY: {
        M* arr = static_cast<M*>(current->structure);
        file << arr->getSize();
        for (size_t i = 0; i < arr->getSize(); ++i) {
          std::string val;
          arr->get(i, val);
          file << " \"" << val << "\"";
        }
        break;
      }
      case StructType::SINGLY_LIST: {
        F* list = static_cast<F*>(current->structure);
        file << list->getLength();
        FNode* node = list->getHead();
        while (node) {
          file << " \"" << node->value << "\"";
          node = node->next;
        }
        break;
      }
      case StructType::DOUBLY_LIST: {
        L* list = static_cast<L*>(current->structure);
        file << list->getLength();
        LNode* node = list->getHead();
        while (node) {
          file << " \"" << node->value << "\"";
          node = node->next;
        }
        break;
      }
      case StructType::STACK: {
        S* stack = static_cast<S*>(current->structure);
        file << stack->getSize();
        // стек как список (от вершины к низу)
        SNode* node = stack->getTop();
        while (node) {
          file << " \"" << node->value << "\"";
          node = node->next;
        }
        break;
      }
      case StructType::QUEUE: {
        Q* queue = static_cast<Q*>(current->structure);
        file << queue->getSize();
        // очередь как список (от начала к концу)
        QNode* node = queue->getHead();
        while (node) {
          file << " \"" << node->value << "\"";
          node = node->next;
        }
        break;
      }
      case StructType::TREE: {
        T* tree = static_cast<T*>(current->structure);
        file << tree->getSize();
        // дерево в порядке BFS для восстановления структуры
        TNode* root = tree->getRoot();
        if (root) {
          std::queue<TNode*> q;
          q.push(root);
          while (!q.empty()) {
            TNode* node = q.front();
            q.pop();
            file << " \"" << node->value << "\"";
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
          }
        }
        break;
      }
      case StructType::SET: {
        Set* set = static_cast<Set*>(current->structure);
        file << set->getSize();

        size_t cap = set->getCapacity();
        for (size_t i = 0; i < cap; ++i) {
          SetNode* node = set->getBucket(i);
          while (node) {
            file << " \"" << node->value << "\"";  // значение в кавычках
            node = node->next;
          }
        }
        break;
      }

      default:
        break;
    }
    file << std::endl;
    current = current->next;
  }

  file.close();
  return OperationRes::SUCCESS;
}

OperationRes loadFromFile(Storage* storage, const std::string& filename) {
  if (!storage) return OperationRes::ERROR;

  std::ifstream file(filename);
  if (!file.is_open()) return OperationRes::ERROR;

  std::string line;
  while (std::getline(file, line)) {
    // пропускаем # и пустые строки
    if (line.empty() || line[0] == '#') continue;

    std::istringstream iss(line);
    int typeInt;
    std::string name;
    size_t size;

    if (!(iss >> typeInt >> name >> size))
      continue;  // пропускаем некорректные строки

    StructType type = static_cast<StructType>(typeInt);
    std::vector<std::string> values;
    std::string value;

    // читаем значения
    while (iss >> value) {
      // убираем кавычки
      if (value.size() >= 2 && value.front() == '"' && value.back() == '"')
        value = value.substr(1, value.length() - 2);

      values.push_back(value);
    }

    void* structure = nullptr;
    switch (type) {
      case StructType::ARRAY: {
        M* arr = new M(name);
        for (const auto& val : values) arr->pushBack(val);

        structure = arr;
        break;
      }
      case StructType::SINGLY_LIST: {
        F* list = new F(name);
        for (const auto& val : values) list->pushBack(val);

        structure = list;
        break;
      }
      case StructType::DOUBLY_LIST: {
        L* list = new L(name);
        for (const auto& val : values) list->pushBack(val);

        structure = list;
        break;
      }
      case StructType::STACK: {
        S* stack = new S(name);
        // тк в файле сохранено от вершины к низу, то
        for (auto it = values.rbegin(); it != values.rend(); ++it) {
          stack->push(*it);
        }
        structure = stack;
        break;
      }
      case StructType::QUEUE: {
        Q* queue = new Q(name);
        for (const auto& val : values) queue->enqueue(val);

        structure = queue;
        break;
      }
      case StructType::TREE: {
        T* tree = new T(name);
        for (const auto& val : values) tree->insert(val);

        structure = tree;
        break;
      }
      case StructType::SET: {
        Set* set = new Set(name);
        for (const auto& val : values) {
          set->add(val);
        }
        structure = set;
        break;
      }

      default:
        continue;
    }

    if (structure) storageAdd(storage, name, type, structure);
  }

  file.close();
  return OperationRes::SUCCESS;
}