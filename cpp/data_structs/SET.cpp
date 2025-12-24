// SETCREATE <имя_структуры>"
// SETADD <имя_структуры> <значение>
// SETDEL <имя_структуры> <значение>
// SET_AT <имя_структуры> <значение>
// SETSIZE <имя_структуры>
// PRINT <имя_структуры>
// DELETE <имя_структуры>
// EXISTS <имя_структуры>

#include <iostream>
#include <string>
#include <utility>

#include "structures.h"

Set::Set(std::string name) : name_(std::move(name)) {
  buckets_ = new SetNode*[capacity_]();
}

Set::~Set() {
  for (size_t i = 0; i < capacity_; ++i) {
    SetNode* current = buckets_[i];
    while (current != nullptr) {
      SetNode* next = current->next;
      delete current;
      current = next;
    }
  }
  delete[] buckets_;
}

// приват хеш-функция
auto Set::hash(const std::string& value) const -> size_t {
  size_t hashVal = 0;
  for (char c : value) {
    hashVal = (hashVal * 31) + c;
  }
  return hashVal % capacity_;
}

// приват метод рехеш
void Set::rehash(size_t newCapacity) {
  auto** newBuckets = new SetNode*[newCapacity]();

  // старые корзины
  for (size_t i = 0; i < capacity_; ++i) {
    SetNode* current = buckets_[i];
    while (current != nullptr) {
      SetNode* next = current->next;

      size_t hashVal = 0;
      for (char c : current->value) {
        hashVal = (hashVal * 31) + c;
      }
      size_t newIndex = hashVal % newCapacity;

      current->next = newBuckets[newIndex];
      newBuckets[newIndex] = current;

      current = next;
    }
  }

  delete[] buckets_;
  buckets_ = newBuckets;
  capacity_ = newCapacity;
}

auto Set::contains(const std::string& value) const -> bool {
  size_t index = hash(value);
  SetNode* current = buckets_[index];
  while (current != nullptr) {
    if (current->value == value) {
      return true;
    }
    current = current->next;
  }
  return false;
}

auto Set::add(const std::string& value) -> OperationRes {
  if (contains(value)) {
    return OperationRes::ERROR;
  }

  // проверка на заполненность
  if (size_ >= capacity_ * 0.75) {
    rehash(capacity_ * 2);
  }

  size_t index = hash(value);
  auto* newNode = new SetNode;
  newNode->value = value;
  newNode->next = buckets_[index];
  buckets_[index] = newNode;
  size_++;

  return OperationRes::SUCCESS;
}

auto Set::remove(const std::string& value) -> OperationRes {
  if (size_ == 0) {
    return OperationRes::EMPTY;
  }

  size_t index = hash(value);
  SetNode* current = buckets_[index];
  SetNode* prev = nullptr;

  while (current != nullptr) {
    if (current->value == value) {
      if (prev != nullptr) {
        prev->next = current->next;
      } else {
        buckets_[index] = current->next;
      }
      delete current;
      size_--;
      return OperationRes::SUCCESS;
    }
    prev = current;
    current = current->next;
  }

  return OperationRes::NOT_FOUND;
}

auto Set::empty() const -> bool { return size_ == 0; }

void Set::print() const {
  std::cout << "Множество '" << name_ << "' (размер: " << size_
            << ", capacity: " << capacity_ << "):" << '\n';

  if (size_ == 0) {
    std::cout << "Множество пусто." << '\n';
    return;
  }

  bool first = true;
  std::cout << "{ ";
  for (size_t i = 0; i < capacity_; ++i) {
    SetNode* current = buckets_[i];
    while (current != nullptr) {
      if (!first) {
        std::cout << ", ";
      }
      std::cout << current->value;
      first = false;
      current = current->next;
    }
  }
  std::cout << " }" << '\n';
}