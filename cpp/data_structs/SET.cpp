// SETCREATE <имя_структуры>"
// SETADD <имя_структуры> <значение>
// SETDEL <имя_структуры> <значение>
// SET_AT <имя_структуры> <значение>
// SETSIZE <имя_структуры>
// PRINT <имя_структуры>
// DELETE <имя_структуры>
// EXISTS <имя_структуры>

#include "structures.h"
#include <iostream>
#include <string>

Set::Set(const std::string& name) : name_(name)
{
    buckets_ = new SetNode*[capacity_]();
}

Set::~Set()
{
    for (size_t i = 0; i < capacity_; ++i) {
        SetNode* current = buckets_[i];
        while (current) {
            SetNode* next = current->next;
            delete current;
            current = next;
        }
    }
    delete[] buckets_;
}

// приват хеш-функция
size_t Set::hash(const std::string& value) const
{
    size_t hashVal = 0;
    for (char c : value) {
        hashVal = hashVal * 31 + c;
    }
    return hashVal % capacity_;
}

// приват метод рехеш
void Set::rehash(size_t newCapacity)
{
    SetNode** newBuckets = new SetNode*[newCapacity]();
    
    // старые корзины
    for (size_t i = 0; i < capacity_; ++i) {
        SetNode* current = buckets_[i];
        while (current) {
            SetNode* next = current->next;

            size_t hashVal = 0;
            for (char c : current->value) hashVal = hashVal * 31 + c;
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

bool Set::contains(const std::string& value) const
{
    size_t index = hash(value);
    SetNode* current = buckets_[index];
    while (current) {
        if (current->value == value) return true;
        current = current->next;
    }
    return false;
}

OperationRes Set::add(const std::string& value)
{
    if (contains(value)) return OperationRes::ERROR;

    // проверка на заполненность
    if (size_ >= capacity_ * 0.75) {
        rehash(capacity_ * 2);
    }

    size_t index = hash(value);
    SetNode* newNode = new SetNode;
    newNode->value = value;
    newNode->next = buckets_[index];
    buckets_[index] = newNode;
    size_++;

    return OperationRes::SUCCESS;
}

OperationRes Set::remove(const std::string& value)
{
    if (size_ == 0) return OperationRes::EMPTY;

    size_t index = hash(value);
    SetNode* current = buckets_[index];
    SetNode* prev = nullptr;

    while (current) {
        if (current->value == value) {
            if (prev) {
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

bool Set::empty() const
{
    return size_ == 0;
}

void Set::print() const
{
    std::cout << "Множество '" << name_ << "' (размер: " << size_<< 
        ", capacity: " << capacity_ << "):" << std::endl;
    
    if (size_ == 0) {
        std::cout << "Множество пусто." << std::endl;
        return;
    }

    bool first = true;
    std::cout << "{ ";
    for (size_t i = 0; i < capacity_; ++i) {
        SetNode* current = buckets_[i];
        while (current) {
            if (!first) std::cout << ", ";
            std::cout << current->value;
            first = false;
            current = current->next;
        }
    }
    std::cout << " }" << std::endl;
}