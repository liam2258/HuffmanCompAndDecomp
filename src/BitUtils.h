#ifndef BIT_UTILS_H
#define BIT_UTILS_H

#include <bitset>
#include <stdexcept>
#include <string>
#include <vector>

std::vector<unsigned char> int_to_bytes(int num);
int byte_to_int(std::vector<unsigned char> bytes);

#endif