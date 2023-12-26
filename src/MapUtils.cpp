#include "MapUtils.h"

/**
 * Converts a map of byte occurrences into a vector of nodes.
 *
 * This function takes a map where the key is a byte value (unsigned char) and
 * the value is the frequency of that byte. It creates a new node for each entry
 * in the map, where the node's value is the byte and the node's frequency is
 * the frequency of the byte. These nodes are then added to a vector.
 *
 * @param occurrences The map of byte occurrences to be converted into nodes.
 * @return A vector of nodes representing the byte occurrences.
 */
std::vector<Node *>
get_occurrence_nodes(std::map<unsigned char, int> &occurrences) {
  std::vector<Node *> nodes;

  for (auto &touple : occurrences) {
    nodes.push_back(new Node(touple.first, touple.second));
  }

  return nodes;
}

/**
 * Creates a map of byte occurrences in a file.
 *
 * This function reads the input file one byte at a time, converting each byte
 * to an unsigned char. It then increments the count of that byte in the
 * occurrences map. The function continues this process until it has read the
 * entire file.
 *
 * @param inputFile The file to be read.
 * @return A map where the key is a byte and the value is the frequency of that
 * byte in the file.
 */
std::map<unsigned char, int> get_occurrences(std::ifstream &inputFile) {
  std::map<unsigned char, int> occurrences;
  unsigned char byte;

  while (inputFile.read(reinterpret_cast<char *>(&byte), sizeof(byte))) {
    occurrences[byte]++;
  }

  return occurrences;
}

/**
 * Calculates the amount of padding needed for the final byte of the compressed
 * data.
 *
 * This function uses the occurrences map and the look-up table to find the
 * exact length in bits the compressed data will be. It then mods this length by
 * 8 and subtracts the result from 8 to find the amount of bits that need to be
 * padded on the final byte.
 *
 * @param intMap The map of byte occurrences.
 * @param stringMap The look-up table mapping bytes to their Huffman codes.
 * @return The number of bits that need to be padded on the final byte.
 */
int get_padding_amount(std::map<unsigned char, int> intMap,
                       std::map<unsigned char, std::string> stringMap) {
  int sum = 0;

  // Iterators that will traverse the maps
  std::map<unsigned char, int>::iterator it1 = intMap.begin();
  std::map<unsigned char, std::string>::iterator it2 = stringMap.begin();

  for (; it1 != intMap.end(); ++it1, ++it2) {

    // Getting how many times a character appears
    int intValue = it1->second;

    // Getting how many binary characters are used to represnt that character
    std::string stringValue = it2->second;

    // Multiplying them to find exactly how many bits a character will take up
    // And adding that to the total bit amount for the file
    int result = intValue * stringValue.size();
    sum += result;
  }

  // Finding how many bits would be needed to pad out the last byte
  // i.e. how many bits shy of 8 are we
  int remainder = 8 - (sum % 8);

  // If no padding is necessary the above line will return 8
  // so we simply return 0 in this case.
  if (remainder == 8) {
    return 0;
  }

  return remainder;
}