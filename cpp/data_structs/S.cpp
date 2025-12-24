// SPUSH <имя> <значение>
// SPOP <name>
// SSIZE <имя>
// PRINT <имя>

#include <iostream>
#include "structures.h"

S::S(const std::string &name) : name_(name) {}

S::~S()
{
    SNode *current = top_;
    while (current) {
        SNode *next = current->next;
        delete current;
        current = next;
    }
}

OperationRes S::push(const std::string &value)
{
    SNode *newNode = new SNode;
    newNode->value = value;
    newNode->next = top_;
    top_ = newNode;
    size_++;
    return OperationRes::SUCCESS;
}

// удалить и запомнить элемент
OperationRes S::pop(std::string &result)
{
    if (!top_) return OperationRes::EMPTY;

    SNode *temp = top_;
    result = temp->value; // сохраняем значение
    top_ = temp->next;    // перемещаем вершину
    delete temp;
    size_--;
    return OperationRes::SUCCESS;
}

void S::print() const
{
    std::cout << "Стек " << name_ << " (размер: " << size_ << "):" << std::endl;
    if (!top_) {
        std::cout << "Стек пуст." << std::endl;
        return;
    }

    std::cout << "ВЕРХ" << std::endl;
    SNode* current = top_;
    while (current) {
        std::cout << "| " << current->value << " |" << std::endl;
        current = current->next;
    }
    std::cout << "НИЗ" << std::endl;
}