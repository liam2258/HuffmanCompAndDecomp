#include "CompUtils.h"

// This function reads the given file 1 byte at a time, converting that byte into bits, and then those bits
// into a string of 1s and 0s. The function then checks if we've reached the end of the file, removing the
// remainder from the bit string if so, finally the bit string is used to traverse the huffman tree.
// When it reaches a value (childless node) it writes the value to file, when it runs out of bits
// it deletes the current bits in the bit string and repeats the process with the next byte until
// no more remain
void decompressHelper(std::ofstream& outputFile, std::ifstream& inputFile, Node* head, int remainder) {
  Node* current = head;
  unsigned char byte;
  std::string bits;
  int byteSize = 8;

  while (inputFile.read(reinterpret_cast<char*>(&byte), sizeof(byte))) {
    bits += std::bitset<8>(byte).to_string(); // Convert the byte to a string of 8 bits

    if (inputFile.eof()) {
      // Delete the remainder bits
      bits = bits.substr(0, bits.size() - remainder);
      byteSize = bits.size() - remainder;
    }

    while (bits.size() >= byteSize) {
      for (char bit : bits) {
        if (bit == '0') {
          current = current->left;
        }
        else if (bit == '1') {
          current = current->right;
        }
        else {
          throw std::invalid_argument("Invalid bit encountered in decompression.");
        }

        if (current->left == nullptr && current->right == nullptr) {
          unsigned char value = current->value;
          outputFile.write(reinterpret_cast<const char*>(&value), sizeof(value));
          current = head;
        }
      }
      bits = bits.substr(byteSize); // Remove the bits from the string
    }
  }
}

// Given the head of a huffman tree, an output file to write to, a input file to read from, and a remainder for how many
// bits to discard at the end of the file, this function decompresses the file back to it's original state prior to compression.
// This is done by utilizing the decompressHelper  function
void decompress(Node* head, std::ofstream& outputFile, std::ifstream& inputFile, int remainder) {
  if (head == nullptr) {
    throw std::invalid_argument("Invalid Huffman tree, head received is null.");
  }
  
  decompressHelper(outputFile, inputFile, head, remainder);
}

// This function is used for the decompression feature of the program, opening and reading a
// provided file, retrieiving the stored poacket information, and then decompressing the data
// itself, the exact steps are detialed in the function
void decompressData(std::string file) {
  std::ifstream inputFile(file, std::ios::binary);
  if (!inputFile) {
    throw std::runtime_error("Failed to open the file.");
  }

  size_t dotPos = file.rfind('.');
  std::string filename = file.substr(0, dotPos);
  std::string givenExtension = file.substr(dotPos + 1);

  // Validate the file type is hcmp
  if (givenExtension != "hcmp") {
    throw std::runtime_error(
        "Invalid file type, please select an hcmp file for decompressing");
  }

  // Get the remainder
  int remainder;
  inputFile.read(reinterpret_cast<char *>(&remainder), sizeof(remainder));

  // Get the extension size
  int extensionSize;
  inputFile.read(reinterpret_cast<char *>(&extensionSize), sizeof(extensionSize));

  // Get the extension (4 bytes in a vector of unsigned char)
  std::vector<unsigned char> extensionData(extensionSize);
  inputFile.read(reinterpret_cast<char *>(extensionData.data()), extensionSize);
  std::string extension(extensionData.begin(), extensionData.end());

  // Get the huffman tree size
  int treeSize;
  inputFile.read(reinterpret_cast<char *>(&treeSize), sizeof(treeSize));

  // Get the huffman tree data (in a vector of unsigned char)
  std::vector<unsigned char> treeData(treeSize);
  inputFile.read(reinterpret_cast<char *>(treeData.data()), treeSize);

  // Rebuild the huffman tree using the tree data
  Node* huffmanHead = treeReconstructor(treeData);

  // Create the output file for writing
  std::ofstream outputFile(filename + "(unzp)." + extension, std::ios::binary);
  if (!outputFile) {
    throw std::runtime_error("Failed to open the output file.");
  }

  // Uncompress remaining data using the huffman tree
  decompress(huffmanHead, outputFile, inputFile, remainder);

  // Delete head node, this will activate the destructor and free all child nodes
  delete huffmanHead;

  std::cout << "Data successfully decompressed." << std::endl;
}

// This function is used for the compression feature of the program, it opens and reads the
// given file twice, once to create the huffman tree for it's encoding, and again to actually
// compress the data into the new file, the exact steps are detailed in the function itself
void compressData(std::string file) {
  std::ifstream inputFile(file, std::ios::binary);
  if (!inputFile) {
    throw std::runtime_error("Failed to open the file.");
  }

  // Retrieve file name, extension, and extension size
  size_t dotPos = file.rfind('.');
  std::string extension = file.substr(dotPos + 1);
  std::string filename = file.substr(0, dotPos);
  int extensionSize = extension.size();

  // Validate the file type is not hcmp
  if (extension == "hcmp") {
    throw std::runtime_error(
        "Invalid file type, hcmp is already compressed");
  }

  // Get a map of characters and how often they occur in the file
  std::map<unsigned char, int> occurrences = getOccurrences(inputFile);
  std::cout << "Retrieved occurrences" << '\n';

  // Reset file read position to the beginning
  inputFile.clear();
  inputFile.seekg(0, std::ios::beg);

  // Get a vector of nodes containing the occurrences of each character
  std::vector<Node *> occurrenceNodes = getOccurrenceNodes(occurrences);
  std::cout << "Retrieved occurrence nodes" << '\n';

  // Create Huffman tree out of nodes
  Node *huffmanHead = createHuffmanTree(occurrenceNodes);
  std::cout << "Created Huffman tree" << '\n';

  // Pack the tree into a vector in preorder form to store in file
  std::vector<unsigned char> packedTree = getTreePacket(huffmanHead);

  // Get the size of the tree to also store in file
  int treeSize = packedTree.size();

  // Create a look up table using the huffman tree
  std::map<unsigned char, std::string> table = createTable(huffmanHead);
  std::cout << "Created table" << '\n';

  // Delete head node, this will activate the destructor and free all child nodes
  delete huffmanHead;
  std::cout << "Deleted Huffman head" << '\n';

  // Write the data to file
  std::ofstream outputFile(filename + ".hcmp", std::ios::binary);
  if (outputFile) {
    // Write the amount of digits used for padding to file
    int paddingNum = getPaddingAmount(occurrences, table);
    outputFile.write(reinterpret_cast<const char *>(&paddingNum), sizeof(paddingNum));

    // Write extension size to file
    outputFile.write(reinterpret_cast<const char *>(&extensionSize), sizeof(extensionSize));

    // Write extension to file
    outputFile.write((extension.c_str()), extensionSize);

    // Write tree size to file
    outputFile.write(reinterpret_cast<const char *>(&treeSize), sizeof(treeSize));

    // Write tree data to file
    outputFile.write(reinterpret_cast<const char *>(packedTree.data()), packedTree.size());

    // Convert data and write to file
    std::string buffer;
    unsigned char byte;

    // While the file being compressed is not empty, read a single byte from the file
    while (inputFile.read(reinterpret_cast<char *>(&byte), sizeof(byte))) {
      // Using the look-up table, convert the byte value into it's string binary code
      buffer += table[byte];


      // If the buffer has at least 8 characters, enough to form a single byte,
      // convert the buffer to bytes and write them to file
      while (buffer.size() >= 8) {
        std::string chunk = buffer.substr(0, 8);

        // Remove the first 8 characters in the buffer
        buffer = buffer.substr(8);

        // Stoi with base 2 is used to convert the string binary code into a byte
        unsigned char value = static_cast<unsigned char>(std::stoi(chunk, nullptr, 2));
        // The byte is now written to file
        outputFile.write(reinterpret_cast<const char *>(&value), sizeof(value));
      }
    }

    // Handle remaining bits in the buffer
    if (!buffer.empty()) {
      // Since the remaining bits are less then 8, pad the with 0s
      buffer += std::string(paddingNum, '0');

      // Convert the remaining bits to a byte
      unsigned char value = static_cast<unsigned char>(std::stoi(buffer, nullptr, 2));

      // Write the last byte to file
      outputFile.write(reinterpret_cast<const char *>(&value), sizeof(value));
    }

    outputFile.close();
    std::cout << "Data successfully compressed." << std::endl;
  } else {
    std::cout << "Failed to open the file." << std::endl;
  }
}