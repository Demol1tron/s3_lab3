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
#include <utility>

#include "structures.h"

L::L(std::string name) : name_(std::move(name)) {}

L::~L() {
  LNode* current = head_;
  while (current != nullptr) {
    LNode* next = current->next;
    delete current;
    current = next;
  }
}

auto L::pushFront(const std::string& value) -> OperationRes {
  auto* newNode = new LNode;
  newNode->value = value;

  if (head_ == nullptr) {  // список пуст
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

auto L::pushBack(const std::string& value) -> OperationRes {
  auto* newNode = new LNode;
  newNode->value = value;

  if (head_ == nullptr) {  // список пуст
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

auto L::pushAfter(LNode* node, const std::string& value) -> OperationRes {
  if (node == nullptr) {
    return OperationRes::ERROR;
  }

  auto* newNode = new LNode;
  newNode->value = value;
  newNode->next = node->next;
  newNode->prev = node;

  if (node->next != nullptr) {
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

auto L::pushBefore(LNode* node, const std::string& value) -> OperationRes {
  if (node == nullptr) {
    return OperationRes::ERROR;
  }

  // случай добавления перед головой
  if (node == head_) {
    return pushFront(value);
  }

  auto* newNode = new LNode;
  newNode->value = value;
  newNode->next = node;
  newNode->prev = node->prev;

  if (node->prev != nullptr) {
    node->prev->next = newNode;
  }
  node->prev = newNode;

  size_++;
  return OperationRes::SUCCESS;
}

auto L::delFront() -> OperationRes {
  if (head_ == nullptr) {
    return OperationRes::EMPTY;
  }

  LNode* temp = head_;
  head_ = head_->next;

  if (head_ != nullptr) {
    head_->prev = nullptr;
  } else {
    tail_ = nullptr;  // список стал пустым
  }
  delete temp;
  size_--;
  return OperationRes::SUCCESS;
}

auto L::delBack() -> OperationRes {
  if (head_ == nullptr) {
    return OperationRes::EMPTY;
  }

  LNode* temp = tail_;
  tail_ = tail_->prev;

  if (tail_ != nullptr) {
    tail_->next = nullptr;
  } else {
    head_ = nullptr;
  }
  delete temp;
  size_--;
  return OperationRes::SUCCESS;
}

auto L::delAfter(LNode* node) -> OperationRes {
  if ((node == nullptr) || (node->next == nullptr)) {
    return OperationRes::ERROR;
  }

  LNode* toDelete = node->next;
  node->next = toDelete->next;

  if (toDelete->next != nullptr) {
    toDelete->next->prev = node;
  } else {
    tail_ = node;
  }

  delete toDelete;
  size_--;
  return OperationRes::SUCCESS;
}

auto L::delBefore(LNode* node) -> OperationRes {
  if ((node == nullptr) || (node->prev == nullptr)) {
    return OperationRes::ERROR;
  }

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

auto L::delByValue(const std::string& value) -> OperationRes {
  if (head_ == nullptr) {
    return OperationRes::EMPTY;
  }

  // ищем узел с значением
  LNode* current = head_;
  while ((current != nullptr) && current->value != value) {
    current = current->next;
  }

  if (current == nullptr) {
    return OperationRes::NOT_FOUND;
  }

  // удаляем найденный узел
  if (current == head_) {
    return delFront();
  }
  if (current == tail_) {
    return delBack();
  }
  current->prev->next = current->next;
  current->next->prev = current->prev;
  delete current;
  size_--;
  return OperationRes::SUCCESS;
}

auto L::find(const std::string& value) const -> LNode* {
  LNode* current = head_;
  while (current != nullptr) {
    if (current->value == value) {
      return current;
    }
    current = current->next;
  }
  return nullptr;
}

auto L::getAt(size_t index) const -> LNode* {
  LNode* current = head_;
  for (size_t i = 0; (current != nullptr) && i < index; ++i) {
    current = current->next;
  }
  return current;
}

auto L::empty() const -> bool { return head_ == nullptr; }

// принт в прямом порядке
void L::printForward() const {
  std::cout << "Двусвязный список " << name_ << " (размер: " << size_
            << ") в прямом порядке:" << '\n';

  if (head_ == nullptr) {
    std::cout << "Список пуст." << '\n';
    return;
  }

  LNode* current = head_;
  std::cout << "NULL <-> ";
  while (current != nullptr) {
    std::cout << current->value;
    if (current->next != nullptr) {
      std::cout << " <-> ";
    }
    current = current->next;
  }
  std::cout << " <-> NULL" << '\n';
}

// принт в обратном порядке
void L::printBackward() const {
  std::cout << "Двусвязный список " << name_ << " (размер: " << size_
            << ") в обратном порядке:" << '\n';

  if (head_ == nullptr) {
    std::cout << "Список пуст." << '\n';
    return;
  }

  LNode* current = tail_;
  std::cout << "NULL <-> ";
  while (current != nullptr) {
    std::cout << current->value;
    if (current->prev != nullptr) {
      std::cout << " <-> ";
    }
    current = current->prev;
  }
  std::cout << " <-> NULL" << '\n';
}