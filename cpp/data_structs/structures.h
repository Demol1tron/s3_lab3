#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <string>

enum class StructType {
  UNKNOWN,      // по умолчанию
  ARRAY,        // M
  SINGLY_LIST,  // F
  DOUBLY_LIST,  // L
  STACK,        // S
  QUEUE,        // Q
  TREE,         // T
  SET           // SET
};

enum class OperationRes { SUCCESS, ERROR, NOT_FOUND, EMPTY, INDEX_OUT };

struct FNode {
  std::string value;
  FNode* next = nullptr;
};

struct LNode {
  std::string value;
  LNode* next = nullptr;
  LNode* prev = nullptr;
};

struct SNode {
  std::string value;
  SNode* next = nullptr;
};

struct QNode {
  std::string value;
  QNode* next = nullptr;
};

struct TNode {
  std::string value;
  TNode* left = nullptr;
  TNode* right = nullptr;
};

struct SetNode {
  std::string value;
  SetNode* next = nullptr;
};

class M {
  std::string* data_ = nullptr;  // массив строк
  size_t size_ = 0;
  size_t capacity_ = 10;
  std::string name_;

  void resize(size_t newCap);

 public:
  M(std::string name);
  ~M();

  std::string getName() const { return name_; }
  size_t getSize() const { return size_; }
  size_t getCapacity() const { return capacity_; }
  bool empty() const;

  OperationRes pushBack(const std::string& value);
  OperationRes pushAt(size_t index, const std::string& value);
  OperationRes get(size_t index, std::string& result) const;
  OperationRes delAt(size_t index);
  OperationRes replace(size_t index, const std::string& value);
  int find(const std::string& value) const;
  void print() const;
  void clear();
};

class F {
  FNode* head_ = nullptr;
  FNode* tail_ = nullptr;
  size_t size_ = 0;
  std::string name_;

 public:
  F(std::string name);
  ~F();

  std::string getName() const { return name_; }
  size_t getLength() const { return size_; }
  bool empty() const;
  FNode* getHead() const { return head_; }

  OperationRes pushFront(const std::string& value);
  OperationRes pushBack(const std::string& value);
  OperationRes pushAfter(FNode* node, const std::string& value);
  OperationRes pushBefore(FNode* node, const std::string& value);
  OperationRes delFront();
  OperationRes delBack();
  OperationRes delAfter(FNode* node);
  OperationRes delBefore(FNode* node);
  OperationRes delByValue(const std::string& value);
  FNode* find(const std::string& value) const;
  void print() const;
};

class L {
  LNode* head_ = nullptr;
  LNode* tail_ = nullptr;
  size_t size_ = 0;
  std::string name_;

 public:
  L(std::string name);
  ~L();

  std::string getName() const { return name_; }
  size_t getLength() const { return size_; }
  bool empty() const;
  LNode* getHead() const { return head_; }
  LNode* getTail() const { return tail_; }

  OperationRes pushFront(const std::string& value);
  OperationRes pushBack(const std::string& value);
  OperationRes pushAfter(LNode* node, const std::string& value);
  OperationRes pushBefore(LNode* node, const std::string& value);
  OperationRes delFront();
  OperationRes delBack();
  OperationRes delAfter(LNode* node);
  OperationRes delBefore(LNode* node);
  OperationRes delByValue(const std::string& value);
  LNode* find(const std::string& value) const;
  LNode* getAt(size_t index) const;
  void printForward() const;
  void printBackward() const;
};

class S {
  SNode* top_ = nullptr;
  size_t size_ = 0;
  std::string name_;

 public:
  S(std::string name);
  ~S();

  std::string getName() const { return name_; }
  size_t getSize() const { return size_; }
  SNode* getTop() const { return top_; }

  bool empty() const;

  OperationRes push(const std::string& value);
  OperationRes pop(std::string& result);
  void print() const;
};

class Q {
  QNode* head_ = nullptr;
  QNode* tail_ = nullptr;
  size_t size_ = 0;
  std::string name_;

 public:
  Q(std::string  name);
  ~Q();

  std::string getName() const { return name_; }
  size_t getSize() const { return size_; }
  QNode* getHead() const { return head_; }

  bool empty() const;

  OperationRes enqueue(const std::string& value);
  OperationRes pop(std::string& result);
  void print() const;
};

class T {
  TNode* root_ = nullptr;
  std::string name_;
  size_t size_ = 0;

  void deleteSubtree(TNode* node);
  bool searchRecursive(const TNode* node, const std::string& value) const;
  bool isFullBinaryTree(const TNode* node) const;
  void printTreeVisual(const TNode* node, const std::string& prefix,
                       bool isLeft) const;
  void printPreOrder(const TNode* node) const;
  void printInOrder(const TNode* node) const;
  void printPostOrder(const TNode* node) const;

 public:
  T(std::string name);
  ~T();

  std::string getName() const { return name_; }
  size_t getSize() const { return size_; }
  TNode* getRoot() const { return root_; }

  bool empty() const;
  bool isFull() const;

  OperationRes insert(const std::string& value);
  OperationRes search(const std::string& value) const;

  void print() const;  // визуал
  void printBFS() const;
};

class Set {
  SetNode** buckets_ = nullptr;
  size_t capacity_ = 16;
  size_t size_ = 0;
  std::string name_;

  size_t hash(const std::string& value) const;
  void rehash(size_t newCapacity);

 public:
  Set(std::string name);
  ~Set();

  std::string getName() const { return name_; }
  size_t getSize() const { return size_; }
  size_t getCapacity() const { return capacity_; }
  SetNode* getBucket(size_t index) const { return buckets_[index]; }

  bool empty() const;

  OperationRes add(const std::string& value);
  OperationRes remove(const std::string& value);
  bool contains(const std::string& value) const;
  void print() const;
};

// для именнованных структур
struct NamedStructure {
  std::string name;
  StructType type = StructType::UNKNOWN;
  void* structure = nullptr;  // указатель на конкретную структуру
  NamedStructure* next = nullptr;

  NamedStructure(std::string n, StructType t, void* s)
      : name(n), type(t), structure(s) {}
};

#endif