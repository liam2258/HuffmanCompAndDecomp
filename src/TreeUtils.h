#ifndef TREE_UTILS_H
#define TREE_UTILS_H

#include "BitUtils.h"
#include "Node.h"
#include <map>
#include <stdexcept>
#include <vector>

void pre_order_packing(Node *head, std::vector<unsigned char> &tree);
std::vector<unsigned char> get_tree_packet(Node *head);
void link_nodes(Node *current, std::vector<Node *> &nodes,
                std::vector<unsigned char> &flags, int &tracker);
Node *tree_reconstructor(std::vector<unsigned char> treePacket);
void huffman_constructor(std::vector<Node *> &nodes);
Node *create_huffman_tree(std::vector<Node *> &nodes);
void find_tree_path(Node *head, std::string path,
                    std::map<unsigned char, std::string> &table);
std::map<unsigned char, std::string> createTable(Node *huffmanHead);

#endif