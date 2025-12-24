// TINSERT <имя_структуры> <значение>
// TSEARCH <имя_структуры> <значение>
// TISFULL <имя_структуры>
// TSIZE <имя_структуры>
// PRINT <имя_структуры>

#include <iostream>
#include <queue>
#include <string>
#include <utility>

#include "structures.h"

T::T(std::string name) : name_(std::move(name)) {}

T::~T() { deleteSubtree(root_); }

// приват рекурсивный метод для удаления
void T::deleteSubtree(TNode* node) {
  if (node == nullptr) {
    return;
  }
  deleteSubtree(node->left);
  deleteSubtree(node->right);
  delete node;
}

// вставка (level-order)
auto T::insert(const std::string& value) -> OperationRes {
  auto* newNode = new TNode;
  newNode->value = value;

  if (root_ == nullptr) {  // дерево пустое
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

    if (current->left == nullptr) {
      current->left = newNode;
      size_++;
      return OperationRes::SUCCESS;
    }
    if (current->right == nullptr) {
      current->right = newNode;
      size_++;
      return OperationRes::SUCCESS;
    }  // у узла уже есть оба ребенка
    q.push(current->left);
    q.push(current->right);
  }
  delete newNode;
  return OperationRes::ERROR;
}

// приват рекурсивный поиск
auto T::searchRecursive(const TNode* node, const std::string& value) const
    -> bool {
  if (node == nullptr) {
    return false;
  }
  if (node->value == value) {
    return true;
  }
  return searchRecursive(node->left, value) ||
         searchRecursive(node->right, value);
}

// публ метод поиска
auto T::search(const std::string& value) const -> OperationRes {
  if (root_ == nullptr) {
    return OperationRes::NOT_FOUND;
  }
  return searchRecursive(root_, value) ? OperationRes::SUCCESS
                                       : OperationRes::NOT_FOUND;
}

// приват проверка на полноту
auto T::isFullBinaryTree(const TNode* node) const -> bool {
  if (node == nullptr) {
    return true;
  }

  // лист
  if ((node->left == nullptr) && (node->right == nullptr)) {
    return true;
  }

  // оба ребенка
  if ((node->left != nullptr) && (node->right != nullptr)) {
    return isFullBinaryTree(node->left) && isFullBinaryTree(node->right);
  }

  // один ребенок
  return false;
}

// публ метод проверки на полноту
auto T::isFull() const -> bool { return isFullBinaryTree(root_); }

// приват метод для визуализации
void T::printTreeVisual(const TNode* node, const std::string& prefix,
                        bool isLeft) const {
  if (node == nullptr) {
    return;
  }

  std::cout << prefix;
  std::cout << (isLeft ? "├── " : "└── ");
  std::cout << node->value << '\n';

  if ((node->left != nullptr) || (node->right != nullptr)) {
    std::string newPrefix = prefix + (isLeft ? "│   " : "    ");
    if (node->left != nullptr) {
      printTreeVisual(node->left, newPrefix, true);
    }
    if (node->right != nullptr) {
      printTreeVisual(node->right, newPrefix, false);
    }
  }
}

// приват Pre-order
void T::printPreOrder(const TNode* node) const {
  if (node == nullptr) {
    return;
  }
  std::cout << node->value << " ";
  printPreOrder(node->left);
  printPreOrder(node->right);
}

// приват In-order
void T::printInOrder(const TNode* node) const {
  if (node == nullptr) {
    return;
  }
  printInOrder(node->left);
  std::cout << node->value << " ";
  printInOrder(node->right);
}

// приват Post-order
void T::printPostOrder(const TNode* node) const {
  if (node == nullptr) {
    return;
  }
  printPostOrder(node->left);
  printPostOrder(node->right);
  std::cout << node->value << " ";
}

// публ метод BFS (Level-order)
void T::printBFS() const {
  if (root_ == nullptr) {
    return;
  }
  std::queue<TNode*> q;
  q.push(root_);

  std::cout << "BFS: ";
  while (!q.empty()) {
    TNode* current = q.front();
    q.pop();
    std::cout << current->value << " ";
    if (current->left != nullptr) {
      q.push(current->left);
    }
    if (current->right != nullptr) {
      q.push(current->right);
    }
  }
  std::cout << '\n';
}

auto T::empty() const -> bool { return root_ == nullptr; }

void T::print() const {
  std::cout << "=== Дерево '" << name_ << "' (размер: " << size_
            << ") ===" << '\n';
  std::cout << "Full Binary Tree: " << (isFull() ? "ДА" : "НЕТ") << '\n';

  if (root_ == nullptr) {
    std::cout << "Дерево пусто." << '\n';
    return;
  }

  std::cout << '\n' << "Визуал:" << '\n';
  printTreeVisual(root_, "", false);

  std::cout << '\n' << "Способами обхода:" << '\n';
  std::cout << "Pre-order: ";
  printPreOrder(root_);
  std::cout << '\n';

  std::cout << "In-order: ";
  printInOrder(root_);
  std::cout << '\n';

  std::cout << "Post-order: ";
  printPostOrder(root_);
  std::cout << '\n';

  printBFS();
}