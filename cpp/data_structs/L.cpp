// LPUSH <имя_структуры> <значение>
// LPUSH <имя_структуры> front <значение>
// LPUSH <имя_структуры> back <значение>
// LPUSHAFTER <имя_структуры> <пред_значение> <значение_после_добавляемое>
// LPUSHBEFORE <имя_структуры> <значение_после> <пред_значение_добавляемое>
// LDEL <имя_структуры> front
// LDEL <имя_структуры> back
// LDEL <имя_структуры> value <значение>
// LDELAFTER <имя_структуры> <пред_значение>
// LDELBEFORE <имя_структуры> <значение_после>
// LGET <имя_структуры> size
// PRINT <имя_структуры>

#include <iostream>

#include "structures.h"

L::L(const std::string& name) : name_(name) {}

L::~L() {
  LNode* current = head_;
  while (current) {
    LNode* next = current->next;
    delete current;
    current = next;
  }
}

OperationRes L::pushFront(const std::string& value) {
  LNode* newNode = new LNode;
  newNode->value = value;

  if (!head_) {  // список пуст
    head_ = newNode;
    tail_ = newNode;
  } else {
    newNode->next = head_;
    head_->prev = newNode;
    head_ = newNode;
  }

  size_++;
  return OperationRes::SUCCESS;
}

OperationRes L::pushBack(const std::string& value) {
  LNode* newNode = new LNode;
  newNode->value = value;

  if (!head_) {  // список пуст
    head_ = newNode;
    tail_ = newNode;
  } else {
    newNode->prev = tail_;
    tail_->next = newNode;
    tail_ = newNode;
  }

  size_++;
  return OperationRes::SUCCESS;
}

OperationRes L::pushAfter(LNode* node, const std::string& value) {
  if (!node) return OperationRes::ERROR;

  LNode* newNode = new LNode;
  newNode->value = value;
  newNode->next = node->next;
  newNode->prev = node;

  if (node->next) {
    node->next->prev = newNode;
  }
  node->next = newNode;

  // если добавляем после хвоста
  if (node == tail_) {
    tail_ = newNode;
  }
  size_++;
  return OperationRes::SUCCESS;
}

OperationRes L::pushBefore(LNode* node, const std::string& value) {
  if (!node) return OperationRes::ERROR;

  // случай добавления перед головой
  if (node == head_) {
    return pushFront(value);
  }

  LNode* newNode = new LNode;
  newNode->value = value;
  newNode->next = node;
  newNode->prev = node->prev;

  if (node->prev) {
    node->prev->next = newNode;
  }
  node->prev = newNode;

  size_++;
  return OperationRes::SUCCESS;
}

OperationRes L::delFront() {
  if (!head_) return OperationRes::EMPTY;

  LNode* temp = head_;
  head_ = head_->next;

  if (head_) {
    head_->prev = nullptr;
  } else {
    tail_ = nullptr;  // список стал пустым
  }
  delete temp;
  size_--;
  return OperationRes::SUCCESS;
}

OperationRes L::delBack() {
  if (!head_) return OperationRes::EMPTY;

  LNode* temp = tail_;
  tail_ = tail_->prev;

  if (tail_) {
    tail_->next = nullptr;
  } else {
    head_ = nullptr;
  }
  delete temp;
  size_--;
  return OperationRes::SUCCESS;
}

OperationRes L::delAfter(LNode* node) {
  if (!node || !node->next) return OperationRes::ERROR;

  LNode* toDelete = node->next;
  node->next = toDelete->next;

  if (toDelete->next) {
    toDelete->next->prev = node;
  } else {
    tail_ = node;
  }

  delete toDelete;
  size_--;
  return OperationRes::SUCCESS;
}

OperationRes L::delBefore(LNode* node) {
  if (!node || !node->prev) return OperationRes::ERROR;

  if (node->prev == head_) {
    return delFront();
  }

  LNode* toDelete = node->prev;
  toDelete->prev->next = node;
  node->prev = toDelete->prev;

  delete toDelete;
  size_--;
  return OperationRes::SUCCESS;
}

OperationRes L::delByValue(const std::string& value) {
  if (!head_) return OperationRes::EMPTY;

  // ищем узел с значением
  LNode* current = head_;
  while (current && current->value != value) {
    current = current->next;
  }

  if (!current) return OperationRes::NOT_FOUND;

  // удаляем найденный узел
  if (current == head_) {
    return delFront();
  } else if (current == tail_) {
    return delBack();
  } else {
    current->prev->next = current->next;
    current->next->prev = current->prev;
    delete current;
    size_--;
    return OperationRes::SUCCESS;
  }
}

LNode* L::find(const std::string& value) const {
  LNode* current = head_;
  while (current) {
    if (current->value == value) return current;
    current = current->next;
  }
  return nullptr;
}

LNode* L::getAt(size_t index) const {
  LNode* current = head_;
  for (size_t i = 0; current && i < index; ++i) {
    current = current->next;
  }
  return current;
}

bool L::empty() const { return !head_; }

// принт в прямом порядке
void L::printForward() const {
  std::cout << "Двусвязный список " << name_ << " (размер: " << size_
            << ") в прямом порядке:" << std::endl;

  if (!head_) {
    std::cout << "Список пуст." << std::endl;
    return;
  }

  LNode* current = head_;
  std::cout << "NULL <-> ";
  while (current) {
    std::cout << current->value;
    if (current->next) std::cout << " <-> ";
    current = current->next;
  }
  std::cout << " <-> NULL" << std::endl;
}

// принт в обратном порядке
void L::printBackward() const {
  std::cout << "Двусвязный список " << name_ << " (размер: " << size_
            << ") в обратном порядке:" << std::endl;

  if (!head_) {
    std::cout << "Список пуст." << std::endl;
    return;
  }

  LNode* current = tail_;
  std::cout << "NULL <-> ";
  while (current) {
    std::cout << current->value;
    if (current->prev) std::cout << " <-> ";
    current = current->prev;
  }
  std::cout << " <-> NULL" << std::endl;
}