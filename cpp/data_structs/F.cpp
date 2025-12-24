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

#include "structures.h"
#include <iostream>

F::F(const std::string &name) : name_(name) {}

F::~F()
{
    FNode *current = head_;
    while (current) {
        FNode *next = current->next;
        delete current;
        current = next;
    }
}

OperationRes F::pushFront(const std::string &value)
{
    FNode *newNode = new FNode;
    newNode->value = value;
    
    if (!head_) { // список пуст
        head_ = newNode;
        tail_ = newNode;
    } else {
        newNode->next = head_;
        head_ = newNode;
    }

    size_++;
    return OperationRes::SUCCESS;
}

OperationRes F::pushBack(const std::string &value)
{
    FNode* newNode = new FNode;
    newNode->value = value;

    if (!head_) {
        head_ = newNode;
        tail_ = newNode;
    } else {
        tail_->next = newNode;
        tail_ = newNode;
    }

    size_++;
    return OperationRes::SUCCESS;
}

OperationRes F::pushAfter(FNode *node, const std::string &value)
{
    if (!node) return OperationRes::ERROR;

    FNode* newNode = new FNode;
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

OperationRes F::pushBefore(FNode *node, const std::string &value)
{
    if (!node) return OperationRes::ERROR;

    // случай перед головой
    if (node == head_) {
        return pushFront(value);
    }

    // ищем предыдущий узел
    FNode *prev = head_;
    while (prev && prev->next != node) {
        prev = prev->next;
    }

    if (!prev) return OperationRes::NOT_FOUND;

    FNode *newNode = new FNode;
    newNode->value = value;
    newNode->next = node;
    prev->next = newNode;

    size_++;
    return OperationRes::SUCCESS;
}

OperationRes F::delFront()
{
    if (!head_) return OperationRes::EMPTY;

    FNode *temp = head_;
    head_ = head_->next;

    // если удаляем последний элемент
    if (tail_ == temp) {
        tail_ = nullptr;
    }
    delete temp;
    size_--;
    return OperationRes::SUCCESS;
}

OperationRes F::delBack()
{
    if (!head_) return OperationRes::EMPTY;

    // в списке один элемент
    if (head_ == tail_) {
        delete head_;
        head_ = nullptr;
        tail_ = nullptr;
    } else {
        // ищем предпоследний элемент
        FNode *current = head_;
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

OperationRes F::delAfter(FNode *node)
{
    if (!node || !node->next) return OperationRes::ERROR;

    FNode *temp = node->next;
    node->next = temp->next;

    // если удаляем хвост
    if (temp == tail_) {
        tail_ = node;
    }

    delete temp;
    size_--;
    return OperationRes::SUCCESS;
}

OperationRes F::delBefore(FNode *node)
{
    if (!node || !head_) return OperationRes::ERROR;
    if (node == head_) return OperationRes::ERROR;

    // случай удаления перед вторым элементом ( удаляем голову)
    if (head_->next == node) {
        return delFront();
    }

    // ищем пред-предыдущий узел
    FNode *prevPrev = head_;
    while (prevPrev && prevPrev->next && prevPrev->next->next != node) {
        prevPrev = prevPrev->next;
    }

    if (!prevPrev || !prevPrev->next) return OperationRes::NOT_FOUND;

    FNode *toDelete = prevPrev->next;
    prevPrev->next = node; // связываем через узел
    delete toDelete;
    size_--;
    return OperationRes::SUCCESS;
}

// удалить по значению (первое вхождение)
OperationRes F::delByValue(const std::string &value)
{
    if (!head_) return OperationRes::EMPTY;

    // если значение в голове
    if (head_->value == value) {
        return delFront();
    }

    // ищем узел со значением и его предыдущий
    FNode *prev = head_;
    while (prev->next && prev->next->value != value) {
        prev = prev->next;
    }

    if (!prev->next) return OperationRes::NOT_FOUND;

    FNode *toDelete = prev->next;
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
FNode* F::find(const std::string &value) const
{
    FNode *current = head_;
    while (current) {
        if (current->value == value) return current;
        current = current->next;
    }
    return nullptr;
}

bool F::empty() const
{
    return !head_;
}

void F::print() const
{
    std::cout << "Односвязный список " << name_ 
              << " (размер: " << size_ << "):" << std::endl;
    
    if (!head_) {
        std::cout << "Список пуст." << std::endl;
        return;
    }

    FNode *current = head_;
    while (current) {
        std::cout << current->value;
        if (current->next) std::cout << " -> ";
        current = current->next;
    }
    std::cout << " -> NULL" << std::endl;
}