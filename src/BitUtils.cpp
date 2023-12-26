#include "BitUtils.h"

/**
 * Converts an integer into a vector of bytes.
 *
 * This function takes an integer as input and converts it into a vector of
 * bytes (unsigned char). The integer is broken down into 4 bytes, with each
 * byte representing 8 bits of the integer. The bytes are ordered in big-endian
 * format, meaning the most significant byte comes first. If the input integer
 * is negative, an invalid_argument exception is thrown.
 *
 * @param num The integer to be converted into bytes.
 * @return A vector of 4 bytes (unsigned char) representing the input integer.
 */
std::vector<unsigned char> int_to_bytes(int num) {
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

/**
 * Converts a vector of bytes into an integer.
 *
 * This function takes a vector of 4 bytes (unsigned char) as input and converts
 * it into an integer. Each byte represents 8 bits of the integer. The function
 * combines these bytes to form the integer. If the input vector does not
 * contain exactly 4 bytes, an invalid_argument exception is thrown.
 *
 * @param bytes The vector of 4 bytes to be converted into an integer.
 * @return The integer represented by the input bytes.
 */
int byte_to_int(std::vector<unsigned char> bytes) {
  if (bytes.size() != 4) {
    throw std::invalid_argument("Input vector must have a size of 4.");
  }
  int value = (static_cast<int>(bytes[0]) << 24) |
              (static_cast<int>(bytes[1]) << 16) |
              (static_cast<int>(bytes[2]) << 8) | static_cast<int>(bytes[3]);
  return value;
}