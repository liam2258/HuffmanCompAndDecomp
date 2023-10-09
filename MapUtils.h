#ifndef MAP_UTILS_H
#define MAP_UTILS_H

#include "Node.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <map>

// Given a map where the key is a byte value (unsigned char) and the hash value is
// how often the value occurs, this function turns the map into a vector of nodes
// This is done by iterating over the map and creating a new node with the values for each entry
std::vector<Node *> getOccurrenceNodes(std::map<unsigned char, int> &occurrences);

// Given a file, the function creates a map where the key is a character in the file and the
// hash value is the number of times it occurs. This is done by iterating over the file,
// converting each char to an unsigned char, and then incrementing it in the occurences map
std::map<unsigned char, int> getOccurrences(std::ifstream &inputFile);

// Function uses the occurences map and look-up table to find the exact length in bits the compressed data
// will be, it then mods it by 8 and subtracts that result from 8 to find the amount of bits that need to
// be padded on the final byte
int getPaddingAmount(std::map<unsigned char, int> intMap, std::map<unsigned char, std::string> stringMap);

#endif