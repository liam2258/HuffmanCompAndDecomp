#include "MapUtils.h"

// Given a map where the key is a byte value (unsigned char) and the hash value
// is how often the value occurs, this function turns the map into a vector of
// nodes This is done by iterating over the map and creating a new node with the
// values for each entry
std::vector<Node *>
getOccurrenceNodes(std::map<unsigned char, int> &occurrences) {
  std::vector<Node *> nodes;

  for (auto &touple : occurrences) {
    nodes.push_back(new Node(touple.first, touple.second));
  }

  return nodes;
}

// Given a file, the function creates a map where the key is a character in the
// file and the hash value is the number of times it occurs. This is done by
// iterating over the file, converting each char to an unsigned char, and then
// incrementing it in the occurences map
std::map<unsigned char, int> getOccurrences(std::ifstream &inputFile) {
  std::map<unsigned char, int> occurrences;
  unsigned char byte;

  while (inputFile.read(reinterpret_cast<char *>(&byte), sizeof(byte))) {
    occurrences[byte]++;
  }

  return occurrences;
}

// Function uses the occurences map and look-up table to find the exact length
// in bits the compressed data will be, it then mods it by 8 and subtracts that
// result from 8 to find the amount of bits that need to be padded on the final
// byte
int getPaddingAmount(std::map<unsigned char, int> intMap,
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