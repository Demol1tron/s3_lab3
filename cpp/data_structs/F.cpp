// FPUSH <имя_структуры> <значение>
// FPUSH <имя_структуры> front <значение>
// FPUSH <имя_структуры> back <значение>
// FPUSHAFTER <имя_структуры> <пред_значение> <значение_после_добавляемое>
// FPUSHBEFORE <имя_структуры> <значение_после> <пред_значение_добавляемое>
// FDEL <имя_структуры> front
// FDEL <имя_структуры> back
// FDEL <имя_структуры> value <значение>
// FDELAFTER <имя_структуры> <пред_значение>
// FDELBEFORE <имя_структуры> <значение>
// FFIND <имя_структуры> <значение>
// FGET <имя_структуры> size
// PRINT <имя_структуры>

#include <iostream>
#include <utility>

#include "structures.h"

F::F(std::string name) : name_(std::move(name)) {}

F::~F() {
  FNode* current = head_;
  while (current != nullptr) {
    FNode* next = current->next;
    delete current;
    current = next;
  }
}

auto F::pushFront(const std::string& value) -> OperationRes {
  auto* newNode = new FNode;
  newNode->value = value;

  if (head_ == nullptr) {  // список пуст
    head_ = newNode;
    tail_ = newNode;
  } else {
    newNode->next = head_;
    head_ = newNode;
  }

  size_++;
  return OperationRes::SUCCESS;
}

auto F::pushBack(const std::string& value) -> OperationRes {
  auto* newNode = new FNode;
  newNode->value = value;

  if (head_ == nullptr) {
    head_ = newNode;
    tail_ = newNode;
  } else {
    tail_->next = newNode;
    tail_ = newNode;
  }

  size_++;
  return OperationRes::SUCCESS;
}

auto F::pushAfter(FNode* node, const std::string& value) -> OperationRes {
  if (node == nullptr) {
    return OperationRes::ERROR;
  }

  auto* newNode = new FNode;
  newNode->value = value;
  newNode->next = node->next;
  node->next = newNode;

  // если добавить после хвоста, обновить tail
  if (node == tail_) {
    tail_ = newNode;
  }
  size_++;
  return OperationRes::SUCCESS;
}

auto F::pushBefore(FNode* node, const std::string& value) -> OperationRes {
  if (node == nullptr) {
    return OperationRes::ERROR;
  }

  // случай перед головой
  if (node == head_) {
    return pushFront(value);
  }

  // ищем предыдущий узел
  FNode* prev = head_;
  while ((prev != nullptr) && prev->next != node) {
    prev = prev->next;
  }

  if (prev == nullptr) {
    return OperationRes::NOT_FOUND;
  }

  auto* newNode = new FNode;
  newNode->value = value;
  newNode->next = node;
  prev->next = newNode;

  size_++;
  return OperationRes::SUCCESS;
}

auto F::delFront() -> OperationRes {
  if (head_ == nullptr) {
    return OperationRes::EMPTY;
  }

  FNode* temp = head_;
  head_ = head_->next;

  // если удаляем последний элемент
  if (tail_ == temp) {
    tail_ = nullptr;
  }
  delete temp;
  size_--;
  return OperationRes::SUCCESS;
}

auto F::delBack() -> OperationRes {
  if (head_ == nullptr) {
    return OperationRes::EMPTY;
  }

  // в списке один элемент
  if (head_ == tail_) {
    delete head_;
    head_ = nullptr;
    tail_ = nullptr;
  } else {
    // ищем предпоследний элемент
    FNode* current = head_;
    while (current->next != tail_) {
      current = current->next;
    }

    delete tail_;
    current->next = nullptr;
    tail_ = current;
  }

  size_--;
  return OperationRes::SUCCESS;
}

auto F::delAfter(FNode* node) -> OperationRes {
  if ((node == nullptr) || (node->next == nullptr)) {
    return OperationRes::ERROR;
  }

  FNode* temp = node->next;
  node->next = temp->next;

  // если удаляем хвост
  if (temp == tail_) {
    tail_ = node;
  }

  delete temp;
  size_--;
  return OperationRes::SUCCESS;
}

auto F::delBefore(FNode* node) -> OperationRes {
  if ((node == nullptr) || (head_ == nullptr)) {
    return OperationRes::ERROR;
  }
  if (node == head_) {
    return OperationRes::ERROR;
  }

  // случай удаления перед вторым элементом ( удаляем голову)
  if (head_->next == node) {
    return delFront();
  }

  // ищем пред-предыдущий узел
  FNode* prevPrev = head_;
  while ((prevPrev != nullptr) && (prevPrev->next != nullptr) &&
         prevPrev->next->next != node) {
    prevPrev = prevPrev->next;
  }

  if ((prevPrev == nullptr) || (prevPrev->next == nullptr)) {
    return OperationRes::NOT_FOUND;
  }

  FNode* toDelete = prevPrev->next;
  prevPrev->next = node;  // связываем через узел
  delete toDelete;
  size_--;
  return OperationRes::SUCCESS;
}

// удалить по значению (первое вхождение)
auto F::delByValue(const std::string& value) -> OperationRes {
  if (head_ == nullptr) {
    return OperationRes::EMPTY;
  }

  // если значение в голове
  if (head_->value == value) {
    return delFront();
  }

  // ищем узел со значением и его предыдущий
  FNode* prev = head_;
  while ((prev->next != nullptr) && prev->next->value != value) {
    prev = prev->next;
  }

  if (prev->next == nullptr) {
    return OperationRes::NOT_FOUND;
  }

  FNode* toDelete = prev->next;
  prev->next = toDelete->next;

  // если удаляем хвост
  if (toDelete == tail_) {
    tail_ = prev;
  }
  delete toDelete;
  size_--;
  return OperationRes::SUCCESS;
}

// поиск узла по значению
auto F::find(const std::string& value) const -> FNode* {
  FNode* current = head_;
  while (current != nullptr) {
    if (current->value == value) {
      return current;
    }
    current = current->next;
  }
  return nullptr;
}

auto F::empty() const -> bool { return head_ == nullptr; }

void F::print() const {
  std::cout << "Односвязный список " << name_ << " (размер: " << size_
            << "):" << '\n';

  if (head_ == nullptr) {
    std::cout << "Список пуст." << '\n';
    return;
  }

  FNode* current = head_;
  while (current != nullptr) {
    std::cout << current->value;
    if (current->next != nullptr) {
      std::cout << " -> ";
    }
    current = current->next;
  }
  std::cout << " -> NULL" << '\n';
}