#ifndef BIT_UTILS_H
#define BIT_UTILS_H

#include <vector>
#include <stdexcept>
#include <bitset>
#include <string>

std::vector<unsigned char> intToBytes(int num);
int byteToInt(std::vector<unsigned char> bytes);

#endif