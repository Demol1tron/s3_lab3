// TINSERT <имя_структуры> <значение>
// TSEARCH <имя_структуры> <значение>
// TISFULL <имя_структуры>
// TSIZE <имя_структуры>
// PRINT <имя_структуры>

#include <iostream>
#include <queue>
#include <string>

#include "structures.h"

T::T(const std::string& name) : name_(name) {}

T::~T() { deleteSubtree(root_); }

// приват рекурсивный метод для удаления
void T::deleteSubtree(TNode* node) {
  if (!node) return;
  deleteSubtree(node->left);
  deleteSubtree(node->right);
  delete node;
}

// вставка (level-order)
OperationRes T::insert(const std::string& value) {
  TNode* newNode = new TNode;
  newNode->value = value;

  if (!root_) {  // дерево пустое
    root_ = newNode;
    size_++;
    return OperationRes::SUCCESS;
  }

  // ищем первый узел с незаполненными детьми
  std::queue<TNode*> q;
  q.push(root_);

  while (!q.empty()) {
    TNode* current = q.front();
    q.pop();

    if (!current->left) {
      current->left = newNode;
      size_++;
      return OperationRes::SUCCESS;
    } else if (!current->right) {
      current->right = newNode;
      size_++;
      return OperationRes::SUCCESS;
    } else {
      // у узла уже есть оба ребенка
      q.push(current->left);
      q.push(current->right);
    }
  }
  delete newNode;
  return OperationRes::ERROR;
}

// приват рекурсивный поиск
bool T::searchRecursive(const TNode* node, const std::string& value) const {
  if (!node) return false;
  if (node->value == value) return true;
  return searchRecursive(node->left, value) ||
         searchRecursive(node->right, value);
}

// публ метод поиска
OperationRes T::search(const std::string& value) const {
  if (!root_) return OperationRes::NOT_FOUND;
  return searchRecursive(root_, value) ? OperationRes::SUCCESS
                                       : OperationRes::NOT_FOUND;
}

// приват проверка на полноту
bool T::isFullBinaryTree(const TNode* node) const {
  if (!node) return true;

  // лист
  if (!node->left && !node->right) return true;

  // оба ребенка
  if (node->left && node->right) {
    return isFullBinaryTree(node->left) && isFullBinaryTree(node->right);
  }

  // один ребенок
  return false;
}

// публ метод проверки на полноту
bool T::isFull() const { return isFullBinaryTree(root_); }

// приват метод для визуализации
void T::printTreeVisual(const TNode* node, const std::string& prefix,
                        bool isLeft) const {
  if (!node) return;

  std::cout << prefix;
  std::cout << (isLeft ? "├── " : "└── ");
  std::cout << node->value << std::endl;

  if (node->left || node->right) {
    std::string newPrefix = prefix + (isLeft ? "│   " : "    ");
    if (node->left) {
      printTreeVisual(node->left, newPrefix, true);
    }
    if (node->right) {
      printTreeVisual(node->right, newPrefix, false);
    }
  }
}

// приват Pre-order
void T::printPreOrder(const TNode* node) const {
  if (!node) return;
  std::cout << node->value << " ";
  printPreOrder(node->left);
  printPreOrder(node->right);
}

// приват In-order
void T::printInOrder(const TNode* node) const {
  if (!node) return;
  printInOrder(node->left);
  std::cout << node->value << " ";
  printInOrder(node->right);
}

// приват Post-order
void T::printPostOrder(const TNode* node) const {
  if (!node) return;
  printPostOrder(node->left);
  printPostOrder(node->right);
  std::cout << node->value << " ";
}

// публ метод BFS (Level-order)
void T::printBFS() const {
  if (!root_) return;
  std::queue<TNode*> q;
  q.push(root_);

  std::cout << "BFS: ";
  while (!q.empty()) {
    TNode* current = q.front();
    q.pop();
    std::cout << current->value << " ";
    if (current->left) q.push(current->left);
    if (current->right) q.push(current->right);
  }
  std::cout << std::endl;
}

bool T::empty() const { return !root_; }

void T::print() const {
  std::cout << "=== Дерево '" << name_ << "' (размер: " << size_
            << ") ===" << std::endl;
  std::cout << "Full Binary Tree: " << (isFull() ? "ДА" : "НЕТ") << std::endl;

  if (!root_) {
    std::cout << "Дерево пусто." << std::endl;
    return;
  }

  std::cout << std::endl << "Визуал:" << std::endl;
  printTreeVisual(root_, "", false);

  std::cout << std::endl << "Способами обхода:" << std::endl;
  std::cout << "Pre-order: ";
  printPreOrder(root_);
  std::cout << std::endl;

  std::cout << "In-order: ";
  printInOrder(root_);
  std::cout << std::endl;

  std::cout << "Post-order: ";
  printPostOrder(root_);
  std::cout << std::endl;

  printBFS();
}