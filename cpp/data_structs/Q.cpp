// QPUSH <имя_структуры> <значение>
// QPOP <имя_структуры>
// QSIZE <имя_структуры>
// PRINT <имя_структуры>

#include <iostream>

#include "structures.h"

Q::Q(const std::string& name) : name_(name) {}

// удалить очередь
Q::~Q() {
  QNode* current = head_;
  while (current != nullptr) {
    QNode* next = current->next;
    delete current;
    current = next;
  }
}

// добавить элемент в очередь
auto Q::enqueue(const std::string& value) -> OperationRes {
  auto* newNode = new QNode;
  newNode->value = value;
  newNode->next = nullptr;  // Важно инициализировать

  if (tail_ == nullptr) {
    head_ = newNode;
    tail_ = newNode;
  } else {
    tail_->next = newNode;
    tail_ = newNode;
  }

  size_++;
  return OperationRes::SUCCESS;
}

auto Q::pop(std::string& result) -> OperationRes {
  if (head_ == nullptr) {
    return OperationRes::EMPTY;
  }

  QNode* temp = head_;
  result = temp->value;
  head_ = head_->next;

  if (head_ == nullptr) {
    tail_ = nullptr;
  }

  delete temp;
  size_--;
  return OperationRes::SUCCESS;
}

void Q::print() const {
  std::cout << "Очередь '" << name_ << "' (размер: " << size_ << "):" << '\n';
  if (head_ == nullptr) {
    std::cout << "Очередь пуста." << '\n';
    return;
  }

  QNode* current = head_;
  std::cout << "head -> ";
  while (current != nullptr) {
    std::cout << current->value;
    if (current->next != nullptr) {
      std::cout << " -> ";
    }
    current = current->next;
  }
  std::cout << " <- tail" << '\n';
}