#include "BitUtils.h" // Include the corresponding header file

// Given an integer the function returns it in byte form via a vector of
// unsigned char
std::vector<unsigned char> intToBytes(int num) {
  if (num < 0) {
    throw std::invalid_argument("Number cannot be negative.");
  }
  std::vector<unsigned char> bytes(4);
  bytes[0] = static_cast<unsigned char>((num >> 24) & 0xFF);
  bytes[1] = static_cast<unsigned char>((num >> 16) & 0xFF);
  bytes[2] = static_cast<unsigned char>((num >> 8) & 0xFF);
  bytes[3] = static_cast<unsigned char>(num & 0xFF);
  return bytes;
}

// Given 4 bytes the function returns the value as an integer
int byteToInt(std::vector<unsigned char> bytes) {
  if (bytes.size() != 4) {
    throw std::invalid_argument("Input vector must have a size of 4.");
  }
  int value = (static_cast<int>(bytes[0]) << 24) |
              (static_cast<int>(bytes[1]) << 16) |
              (static_cast<int>(bytes[2]) << 8) | static_cast<int>(bytes[3]);
  return value;
}