// SPUSH <имя> <значение>
// SPOP <name>
// SSIZE <имя>
// PRINT <имя>

#include <iostream>
#include <utility>

#include "structures.h"

S::S(std::string name) : name_(std::move(name)) {}

S::~S() {
  SNode* current = top_;
  while (current != nullptr) {
    SNode* next = current->next;
    delete current;
    current = next;
  }
}

auto S::push(const std::string& value) -> OperationRes {
  auto* newNode = new SNode;
  newNode->value = value;
  newNode->next = top_;
  top_ = newNode;
  size_++;
  return OperationRes::SUCCESS;
}

// удалить и запомнить элемент
auto S::pop(std::string& result) -> OperationRes {
  if (top_ == nullptr) {
    return OperationRes::EMPTY;
  }

  SNode* temp = top_;
  result = temp->value;  // сохраняем значение
  top_ = temp->next;     // перемещаем вершину
  delete temp;
  size_--;
  return OperationRes::SUCCESS;
}

void S::print() const {
  std::cout << "Стек " << name_ << " (размер: " << size_ << "):" << '\n';
  if (top_ == nullptr) {
    std::cout << "Стек пуст." << '\n';
    return;
  }

  std::cout << "ВЕРХ" << '\n';
  SNode* current = top_;
  while (current != nullptr) {
    std::cout << "| " << current->value << " |" << '\n';
    current = current->next;
  }
  std::cout << "НИЗ" << '\n';
}