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
  while (current) {
    QNode* next = current->next;
    delete current;
    current = next;
  }
}

// добавить элемент в очередь
OperationRes Q::enqueue(const std::string& value) {
  QNode* newNode = new QNode;
  newNode->value = value;
  newNode->next = nullptr;  // Важно инициализировать

  if (!tail_) {
    head_ = newNode;
    tail_ = newNode;
  } else {
    tail_->next = newNode;
    tail_ = newNode;
  }

  size_++;
  return OperationRes::SUCCESS;
}

OperationRes Q::pop(std::string& result) {
  if (!head_) return OperationRes::EMPTY;

  QNode* temp = head_;
  result = temp->value;
  head_ = head_->next;

  if (!head_) {
    tail_ = nullptr;
  }

  delete temp;
  size_--;
  return OperationRes::SUCCESS;
}

void Q::print() const {
  std::cout << "Очередь '" << name_ << "' (размер: " << size_
            << "):" << std::endl;
  if (!head_) {
    std::cout << "Очередь пуста." << std::endl;
    return;
  }

  QNode* current = head_;
  std::cout << "head -> ";
  while (current) {
    std::cout << current->value;
    if (current->next) std::cout << " -> ";
    current = current->next;
  }
  std::cout << " <- tail" << std::endl;
}