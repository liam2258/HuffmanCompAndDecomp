#ifndef NODE_H
#define NODE_H

// Define the node class to use in the huffman tree
class Node {
public:
  unsigned char value;
  int frequency;
  Node *left = nullptr;
  Node *right = nullptr;

  Node() {}

  Node(char val, int freq) {
    value = val;
    frequency = freq;
  }

  // Deconstructor frees all child nodes upon descruction of parent node
  ~Node() {
    if (left != nullptr) {
      delete left;
      left = nullptr;
    }

    if (right != nullptr) {
      delete right;
      right = nullptr;
    }
  }
};

#endif // NODE_H