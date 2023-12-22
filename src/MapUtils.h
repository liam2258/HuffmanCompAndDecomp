#ifndef MAP_UTILS_H
#define MAP_UTILS_H

#include "Node.h"
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

std::vector<Node *>
getOccurrenceNodes(std::map<unsigned char, int> &occurrences);
std::map<unsigned char, int> getOccurrences(std::ifstream &inputFile);
int getPaddingAmount(std::map<unsigned char, int> intMap,
                     std::map<unsigned char, std::string> stringMap);

#endif