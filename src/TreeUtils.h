#ifndef TREE_UTILS_H
#define TREE_UTILS_H

#include "BitUtils.h"
#include "Node.h"
#include <vector>
#include <stdexcept>
#include <map>

// Given the head of a tree, traverses the tree in preorder form and returns a
// packet
// The packet contains the tree in the following format [1 byte: value][4 bytes:
// frequency][1 byte: child flag], with the child flag indicating if the node
// has a left child, right child, or none
void preOrderPacking(Node *head, std::vector<unsigned char> &tree);

// Given the head of a huffman tree, this function creates a packet of bytes
// that will be stored in the compressed file, this will be used to rebuild the
// tree during decompression
std::vector<unsigned char> getTreePacket(Node *head);

// A helper function for treeReconstructor, based on the node and associated flag it links the nodes together.
// The tracker variable keeps note of the index during each recursive call
void linkNodes(Node *current, std::vector<Node *> &nodes, std::vector<unsigned char> &flags, int &tracker);

// Given a byte packet which contains information about every tree in the node
// and an associated flag, which describes how many children each node has,
// Construct a tree
Node *treeReconstructor(std::vector<unsigned char> treePacket);

// A helper function for createHuffmanTree, it recursively takes the least
// frequent nodes, gives them a parent node that combines their frequencies, and
// then removes the two child nodes and adds the parent to the node vector. This
// continues until only two nodes are left. (For more info please see readme)
void huffmanConstructor(std::vector<Node *> &nodes);

// Given a vector of Node pointers, the function organizes them into a huffman tree
Node *createHuffmanTree(std::vector<Node *> &nodes);

// A helper function for create table, it creates a map that keeps track of the binary path
// for each value in the huffman tree. It does this by recursively traversing the tree in
// preorder form while passsing a copy of the path string in every function call,
// appending it with 0 if it traverses left, and 1 if it traverses right, then commiting
// that path to the map when finding a node with no child nodes
void findTreePath(Node *head, std::string path, std::map<unsigned char, std::string> &table);

// Given the head of a huffman tree the function returns a look up table as a map,
// where the key is a node value and the hash value is the binary path to reach said node
// in string form. This is accomplished by leverageing the helper function findTreePath
std::map<unsigned char, std::string> createTable(Node *huffmanHead);

#endif // TREE_UTILS_H