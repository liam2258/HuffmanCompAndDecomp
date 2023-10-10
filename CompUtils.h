#ifndef COMP_UTILS_H
#define COMP_UTILS_H

#include "BitUtils.h"
#include "TreeUtils.h"
#include "MapUtils.h"
#include <cstring>

// This function reads the given file 1 byte at a time, converting that byte into bits, and then those bits
// into a string of 1s and 0s. The function then checks if we've reached the end of the file, removing the
// remainder from the bit string if so, finally the bit string is used to traverse the huffman tree.
// When it reaches a value (childless node) it writes the value to file, when it runs out of bits
// it deletes the current bits in the bit string and repeats the process with the next byte until
// no more remain
void decompressHelper(std::ofstream& outputFile, std::ifstream& inputFile, Node* head, int remainder);

// Given the head of a huffman tree, an output file to write to, a input file to read from, and a remainder for how many
// bits to discard at the end of the file, this function decompresses the file back to it's original state prior to compression.
// This is done by utilizing the decompressHelper  function
void decompress(Node* head, std::ofstream& outputFile, std::ifstream& inputFile, int remainder);

// This function is used for the decompression feature of the program, opening and reading a
// provided file, retrieiving the stored poacket information, and then decompressing the data
// itself, the exact steps are detialed in the function
void decompressData(std::string file);

// This function is used for the compression feature of the program, it opens and reads the
// given file twice, once to create the huffman tree for it's encoding, and again to actually
// compress the data into the new file, the exact steps are detailed in the function itself
void compressData(std::string file);


#endif