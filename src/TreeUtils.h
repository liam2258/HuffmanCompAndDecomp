#ifndef TREE_UTILS_H
#define TREE_UTILS_H

#include "BitUtils.h"
#include "Node.h"
#include <vector>
#include <stdexcept>
#include <map>

void preOrderPacking(Node *head, std::vector<unsigned char> &tree);
std::vector<unsigned char> getTreePacket(Node *head);
void linkNodes(Node *current, std::vector<Node *> &nodes, std::vector<unsigned char> &flags, int &tracker);
Node *treeReconstructor(std::vector<unsigned char> treePacket);
void huffmanConstructor(std::vector<Node *> &nodes);
Node *createHuffmanTree(std::vector<Node *> &nodes);
void findTreePath(Node *head, std::string path, std::map<unsigned char, std::string> &table);
std::map<unsigned char, std::string> createTable(Node *huffmanHead);

#endif