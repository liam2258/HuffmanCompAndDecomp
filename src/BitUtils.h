#ifndef BIT_UTILS_H
#define BIT_UTILS_H

#include <bitset>
#include <stdexcept>
#include <string>
#include <vector>

std::vector<unsigned char> intToBytes(int num);
int byteToInt(std::vector<unsigned char> bytes);

#endif