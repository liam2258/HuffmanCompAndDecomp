#ifndef TREE_UTILS_H
#define TREE_UTILS_H

#include "BitUtils.h"
#include "Node.h"
#include <cstring>
#include <iostream>
#include <map>
#include <vector>
#include <stdexcept>

// Given the head of a tree, traverses the tree in preorder form and returns a
// packet
// The packet contains the tree in the following format [1 byte: value][4 bytes:
// frequency][1 byte: child flag], with the child flag indicating if the node
// has a left child, right child, or none
void preOrderPacking(Node *head, std::vector<unsigned char> &tree) {
  if (head == nullptr) {
    return;
  }
  // First add the node value
  tree.push_back(head->value);

  // Second add the frequency

  // Convert the frequency integer into a vector of unsigned char
  // Each item in the vector is once of 4 bytes that make up an integer
  std::vector<unsigned char> frequency = intToBytes(head->frequency);

  // Add the frequency vector to the tree vector
  tree.insert(tree.end(), frequency.begin(), frequency.end());

  // Lasly based on if the node has left or right or no children add an identifier
  if (head->left == nullptr && head->right == nullptr) {
    tree.push_back('A');
  } else if (head->left == nullptr) {
    tree.push_back('B');
  } else if (head->right == nullptr) {
    tree.push_back('C');
  } else {
    tree.push_back('D');
  }
  preOrderPacking(head->left, tree);
  preOrderPacking(head->right, tree);
}

// Given the head of a huffman tree, this function creates a packet of bytes
// that will be stored in the compressed file, this will be used to rebuild the
// tree during decompression
std::vector<unsigned char> getTreePacket(Node *head) {
  if (head == nullptr) {
    throw std::invalid_argument("Tree is empty.");
  }

  std::vector<unsigned char> tree;

  preOrderPacking(head, tree);

  return tree;
}

// A helper function for treeReconstructor, based on the node and associated flag it links the nodes together.
// The tracker variable keeps note of the index during each recursive call
void linkNodes(Node *current, std::vector<Node *> &nodes, std::vector<unsigned char> &flags, int &tracker) {
  switch (flags[tracker]) {
  case 'A': // In the case where a has no children
    return;
  case 'B': // In the case where a node only has a right child
    tracker++;
    current->right = nodes[tracker];
    linkNodes(nodes[tracker], nodes, flags, tracker);
    break;
  case 'C': // In the case where a node only has a left child
    tracker++;
    current->left = nodes[tracker];
    linkNodes(nodes[tracker], nodes, flags, tracker);
    break;
  case 'D': // In the case where a node has a left and right child
    tracker++;
    current->left = nodes[tracker];
    linkNodes(nodes[tracker], nodes, flags, tracker);
    tracker++;
    current->right = nodes[tracker];
    linkNodes(nodes[tracker], nodes, flags, tracker);
    break;
  }
}

// Given a byte packet which contains information about every tree in the node
// and an associated flag, which describes how many children each node has,
// Construct a tree
Node *treeReconstructor(std::vector<unsigned char> treePacket) {
  std::vector<Node *> nodes;
  std::vector<unsigned char> flags;

  // Check if the packet has a valid size
  if (!treePacket.size() || treePacket.size() % 6) {
    throw std::invalid_argument("Invalid packet size");
  }

  // Extract the nodes and the flags (Every bundle has 6 bytes where the 1st
  // byte is the node value, the 2-5th are the frequency, and the 6th is a flag)
  for (int i = 0; i <= treePacket.size() - 6; i += 6) {
    // Grab bytes 2-5 which make up the frequency integer value and convert it to an integer
    std::vector<unsigned char> frequencyBytes {treePacket[i + 1], treePacket[i + 2], treePacket[i + 3], treePacket[i + 4]};
    int frequency = byteToInt(frequencyBytes);

    // Create a new node using the value the node is representing (treePacket[i]) and the frequency integer we just created
    // Then add the node to the nodes vector
    nodes.push_back(new Node(treePacket[i], frequency));

    // Retrieve the flag, which identifies how many children each node has, and add it to the flag vector
    flags.push_back(treePacket[i + 5]);
  }

  // Tracker used to keep track of index during recursive function calling
  int tracker = 0;

  // Use the created nodes and flags vector to link the nodes together into a tree
  linkNodes(nodes[0], nodes, flags, tracker);

  return nodes[0];
}

// A helper function for createHuffmanTree, it recursively takes the least
// frequent nodes, gives them a parent node that combines their frequencies, and
// then removes the two child nodes and adds the parent to the node vector. This
// continues until only two nodes are left. (For more info please see readme)
void huffmanConstructor(std::vector<Node *> &nodes) {
  // Set the default frequencies to maximum
  int lowestFreq1 = 2147483647;
  int lowestFreq2 = 2147483647;
  int minIndex1 = -1;
  int minIndex2 = -1;

  // Get the 2 lowest frequency nodes
  for (int i = 0; i < nodes.size(); ++i) {
    if (nodes[i]->frequency < lowestFreq1) {
      lowestFreq2 = lowestFreq1;
      minIndex2 = minIndex1;

      lowestFreq1 = nodes[i]->frequency;
      minIndex1 = i;
    } else if (nodes[i]->frequency < lowestFreq2) {
      lowestFreq2 = nodes[i]->frequency;
      minIndex2 = i;
    }
  }

  Node *minNode1 = nodes[minIndex1];
  Node *minNode2 = nodes[minIndex2];

  // Create a new node that combines the two frequencies of the child nodes, assigning the smaller node as it's left child and the larger node as it's right child
  int newFreq = minNode1->frequency + minNode2->frequency;
  Node *newNode = new Node('Z', newFreq);
  newNode->left = minNode1;
  newNode->right = minNode2;

  //Check which index location the min value is located, if minIndex1 is larger delete it first if not delete it second.
  // This is done to keep the indexes from changing after the first erasure
  if (minIndex1 > minIndex2) {
    nodes.erase(nodes.begin() + minIndex1);
    nodes.erase(nodes.begin() + minIndex2);
  } else {
    nodes.erase(nodes.begin() + minIndex2);
    nodes.erase(nodes.begin() + minIndex1);
  }
  nodes.push_back(newNode);
  if (nodes.size() > 2) {
    huffmanConstructor(nodes);
  }
}

// Given a vector of Node pointers, the function organizes them into a huffman tree
Node *createHuffmanTree(std::vector<Node *> &nodes) {
  if (nodes.size() < 1) {
    throw std::invalid_argument("Empty vector");
  }

  // If the node is only of size one, give it a parent and then return the head
  if (nodes.size() < 2) {
    Node *temp = new Node();
    temp->frequency = 0;
    temp->left = nodes[0];
    return temp;
  }
  huffmanConstructor(nodes);
  // Once there are only two unconnected nodes left, create the final parent
  // node and make the less used node the left child and the more used node the
  // right child
  int frequency = nodes[0]->frequency + nodes[1]->frequency;
  Node *head = new Node(0, frequency);
  if (nodes[0]->frequency < nodes[1]->frequency) {
    head->left = nodes[0];
    head->right = nodes[1];
  } else {
    head->left = nodes[1];
    head->right = nodes[0];
  }
  return head;
}

#endif // TREE_UTILS_H