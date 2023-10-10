#ifndef BIT_UTILS_H
#define BIT_UTILS_H

#include <vector>
#include <stdexcept>
#include <bitset>
#include <string>

// Given an integer the function returns it in byte form via a vector of unsigned
// char
std::vector<unsigned char> intToBytes(int num);

// Given 4 bytes the function returns the value as an integer
int byteToInt(std::vector<unsigned char> bytes);

#endif // BIT_UTILS_H