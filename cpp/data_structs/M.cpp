// MPUSH <имя_структуры> <значение>
// MPUSH <имя_структуры> <индекс> <значение>
// MGET <имя_структуры> <индекс>
// MDEL <имя_структуры> <индекс>
// MREPLACE <имя_структуры> <индекс> <значение>
// MFIND <имя_структуры> <значение>
// MSIZE <имя_структуры>
// PRINT <имя_структуры>

#include <iostream>

#include "structures.h"

M::M(const std::string& name) : name_(name) {
  data_ = new std::string[capacity_];
}

M::~M() { delete[] data_; }

void M::resize(size_t newCap) {
  if (newCap <= capacity_) return;

  std::string* newData = new std::string[newCap];
  for (size_t i = 0; i < size_; ++i) {
    newData[i] = data_[i];
  }
  delete[] data_;
  data_ = newData;
  capacity_ = newCap;
}

OperationRes M::pushBack(const std::string& value) {
  if (size_ >= capacity_) {
    resize(capacity_ * 2);
  }
  data_[size_++] = value;
  return OperationRes::SUCCESS;
}

OperationRes M::pushAt(size_t index, const std::string& value) {
  if (index > size_) return OperationRes::INDEX_OUT;

  if (index == size_) return pushBack(value);

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

OperationRes M::get(size_t index, std::string& result) const {
  if (index >= size_) return OperationRes::INDEX_OUT;
  result = data_[index];
  return OperationRes::SUCCESS;
}

OperationRes M::delAt(size_t index) {
  if (index >= size_) return OperationRes::INDEX_OUT;
  if (size_ == 0) return OperationRes::EMPTY;

  // сдвиг влево
  for (size_t i = index; i < size_ - 1; ++i) {
    data_[i] = data_[i + 1];
  }
  size_--;
  return OperationRes::SUCCESS;
}

OperationRes M::replace(size_t index, const std::string& value) {
  if (index >= size_) return OperationRes::INDEX_OUT;
  data_[index] = value;
  return OperationRes::SUCCESS;
}

void M::print() const {
  std::cout << "Массив " << name_ << " (размер: " << size_
            << ", емкость: " << capacity_ << ")" << std::endl;
  if (size_ == 0) {
    std::cout << "Массив пуст." << std::endl;
    return;
  }
  std::cout << "Элементы массива:\n";
  for (size_t i = 0; i < size_; ++i) {
    std::cout << "[" << i << "] = " << data_[i] << std::endl;
  }
}

int M::find(const std::string& value) const {
  for (size_t i = 0; i < size_; ++i) {
    if (data_[i] == value) return i;
  }
  return -1;
}

bool M::empty() const { return size_ == 0; }

void M::clear() { size_ = 0; }