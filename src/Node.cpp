#include "Node.h"

// Define constructors
Node::Node() : value(0), frequency(0), left(nullptr), right(nullptr) {
}

Node::Node(char val, int freq) : value(val), frequency(freq), left(nullptr), right(nullptr) {
}

// Deconstructor frees all child nodes upon descruction of parent node
Node::~Node() {
  if (left != nullptr) {
    delete left;
    left = nullptr;
  }

  if (right != nullptr) {
    delete right;
    right = nullptr;
  }
}