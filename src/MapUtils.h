#ifndef MAP_UTILS_H
#define MAP_UTILS_H

#include "Node.h"
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

std::vector<Node *>
get_occurrence_nodes(std::map<unsigned char, int> &occurrences);
std::map<unsigned char, int> get_occurrences(std::ifstream &inputFile);
int get_padding_amount(std::map<unsigned char, int> intMap,
                       std::map<unsigned char, std::string> stringMap);

#endif