// MPUSH <имя_структуры> <значение>
// MPUSH <имя_структуры> <индекс> <значение>
// MGET <имя_структуры> <индекс>
// MDEL <имя_структуры> <индекс>
// MREPLACE <имя_структуры> <индекс> <значение>
// MFIND <имя_структуры> <значение>
// MSIZE <имя_структуры>
// PRINT <имя_структуры>

#include <iostream>
#include <utility>

#include "structures.h"

M::M(std::string name) : name_(std::move(name)) {
  data_ = new std::string[capacity_];
}

M::~M() { delete[] data_; }

void M::resize(size_t newCap) {
  if (newCap <= capacity_) {
    return;
  }

  auto* newData = new std::string[newCap];
  for (size_t i = 0; i < size_; ++i) {
    newData[i] = data_[i];
  }
  delete[] data_;
  data_ = newData;
  capacity_ = newCap;
}

auto M::pushBack(const std::string& value) -> OperationRes {
  if (size_ >= capacity_) {
    resize(capacity_ * 2);
  }
  data_[size_++] = value;
  return OperationRes::SUCCESS;
}

auto M::pushAt(size_t index, const std::string& value) -> OperationRes {
  if (index > size_) {
    return OperationRes::INDEX_OUT;
  }

  if (index == size_) {
    return pushBack(value);
  }

  if (size_ >= capacity_) {
    resize(capacity_ * 2);
  }

  // сдвиг вправо
  for (size_t i = size_; i > index; --i) {
    data_[i] = data_[i - 1];
  }
  data_[index] = value;
  size_++;
  return OperationRes::SUCCESS;
}

auto M::get(size_t index, std::string& result) const -> OperationRes {
  if (index >= size_) {
    return OperationRes::INDEX_OUT;
  }
  result = data_[index];
  return OperationRes::SUCCESS;
}

auto M::delAt(size_t index) -> OperationRes {
  if (index >= size_) {
    return OperationRes::INDEX_OUT;
  }
  if (size_ == 0) {
    return OperationRes::EMPTY;
  }

  // сдвиг влево
  for (size_t i = index; i < size_ - 1; ++i) {
    data_[i] = data_[i + 1];
  }
  size_--;
  return OperationRes::SUCCESS;
}

auto M::replace(size_t index, const std::string& value) -> OperationRes {
  if (index >= size_) {
    return OperationRes::INDEX_OUT;
  }
  data_[index] = value;
  return OperationRes::SUCCESS;
}

void M::print() const {
  std::cout << "Массив " << name_ << " (размер: " << size_
            << ", емкость: " << capacity_ << ")" << '\n';
  if (size_ == 0) {
    std::cout << "Массив пуст." << '\n';
    return;
  }
  std::cout << "Элементы массива:\n";
  for (size_t i = 0; i < size_; ++i) {
    std::cout << "[" << i << "] = " << data_[i] << '\n';
  }
}

auto M::find(const std::string& value) const -> int {
  for (size_t i = 0; i < size_; ++i) {
    if (data_[i] == value) {
      return i;
    }
  }
  return -1;
}

auto M::empty() const -> bool { return size_ == 0; }

void M::clear() { size_ = 0; }