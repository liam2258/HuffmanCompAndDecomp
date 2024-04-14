#include "CompUtils.h"

/**
 * This function decompresses a file that was compressed using Huffman coding.
 *
 * It reads the input file one byte at a time, converting each byte into a
 * string of 1s and 0s (bits). It then uses this bit string to traverse the
 * Huffman tree. When it reaches a leaf node (a node with no children), it
 * writes the value of that node to the output file.
 *
 * If it reaches the end of the file, it removes the remainder bits from the bit
 * string. The process is repeated until there are no more bytes left in the
 * input file.
 *
 * @param outputFile The file where the decompressed data will be written.
 * @param inputFile The compressed file to be decompressed.
 * @param head The root of the Huffman tree used for decompression.
 * @param remainder The number of remainder bits in the last byte of the input
 * file.
 */
void decompress_helper(std::ofstream &outputFile, std::ifstream &inputFile,
                       Node *head, int remainder) {
  Node *current = head;
  unsigned char byte;
  std::string bits;
  std::string::size_type byteSize = 8; // Change to std::string::size_type

  while (inputFile.read(reinterpret_cast<char *>(&byte), sizeof(byte))) {
    bits += std::bitset<8>(byte)
                .to_string(); // Convert the byte to a string of 8 bits

    if (inputFile.eof()) {
      // Delete the remainder bits
      bits = bits.substr(0, bits.size() - remainder);
      byteSize = bits.size() - remainder;
    }

    while (bits.size() >= byteSize) {
      for (char bit : bits) {
        if (bit == '0') {
          current = current->left;
        } else if (bit == '1') {
          current = current->right;
        } else {
          throw std::invalid_argument(
              "Invalid bit encountered in decompression.");
        }

        if (current->left == nullptr && current->right == nullptr) {
          unsigned char value = current->value;
          outputFile.write(reinterpret_cast<const char *>(&value),
                           sizeof(value));
          current = head;
        }
      }
      bits = bits.substr(byteSize); // Remove the bits from the string
    }
  }
}


/**
 * Decompresses a file that was compressed using Huffman coding.
 *
 * This function is a wrapper for the decompress_helper function. It first
 * checks if the head of the Huffman tree is null. If it is, it throws an
 * invalid_argument exception. Otherwise, it calls the decompress_helper
 * function to decompress the file.
 *
 * @param head The root of the Huffman tree used for decompression.
 * @param outputFile The file where the decompressed data will be written.
 * @param inputFile The compressed file to be decompressed.
 * @param remainder The number of remainder bits in the last byte of the input
 * file.
 */
void decompress(Node *head, std::ofstream &outputFile, std::ifstream &inputFile,
                int remainder) {
  if (head == nullptr) {
    throw std::invalid_argument("Invalid Huffman tree, head received is null.");
  }

  decompress_helper(outputFile, inputFile, head, remainder);
}

/**
 * Decompresses a Huffman-compressed file.
 *
 * This function opens the input file in binary mode and checks if it was opened
 * successfully. If not, it throws a runtime_error exception.
 *
 * It then retrieves the name and extension of the input file. If the extension
 * is not "hcmp" (indicating a Huffman-compressed file), it throws a
 * runtime_error exception.
 *
 * After validating the file, it calls the decompress function to decompress the
 * data.
 *
 * @param file The path to the Huffman-compressed file to be decompressed.
 */
void decompress_data(std::string file) {
  std::ifstream inputFile(file, std::ios::binary);
  if (!inputFile) {
    throw std::runtime_error("Failed to open the file.");
  }

  size_t dotPos = file.rfind('.');
  std::string filename = file.substr(0, dotPos);
  std::string givenExtension = file.substr(dotPos + 1);

  if (givenExtension != "hcmp") {
    throw std::runtime_error(
        "Invalid file type, please select an hcmp file for decompressing");
  }

  int remainder;
  inputFile.read(reinterpret_cast<char *>(&remainder), sizeof(remainder));

  int extensionSize;
  inputFile.read(reinterpret_cast<char *>(&extensionSize),
                 sizeof(extensionSize));

  // Get the extension (4 bytes in a vector of unsigned char)
  std::vector<unsigned char> extensionData(extensionSize);
  inputFile.read(reinterpret_cast<char *>(extensionData.data()), extensionSize);
  std::string extension(extensionData.begin(), extensionData.end());

  int treeSize;
  inputFile.read(reinterpret_cast<char *>(&treeSize), sizeof(treeSize));

  std::vector<unsigned char> treeData(treeSize);
  inputFile.read(reinterpret_cast<char *>(treeData.data()), treeSize);

  Node *huffmanHead = tree_reconstructor(treeData);

  std::ofstream outputFile(filename + "(unzp)." + extension, std::ios::binary);
  if (!outputFile) {
    throw std::runtime_error("Failed to open the output file.");
  }

  decompress(huffmanHead, outputFile, inputFile, remainder);

  delete huffmanHead;

  std::cout << "Data successfully decompressed." << std::endl;
}

/**
 * Compresses a file using Huffman coding.
 *
 * This function opens the input file in binary mode and checks if it was opened
 * successfully. If not, it throws a runtime_error exception.
 *
 * It then retrieves the name and extension of the input file. If the extension
 * is "hcmp" (indicating a Huffman-compressed file), it throws a runtime_error
 * exception.
 *
 * After validating the file, it calculates the frequency of each byte in the
 * file and uses this information to build a Huffman tree. It then uses this
 * tree to compress the data in the file.
 *
 * @param file The path to the file to be compressed.
 */
void compress_data(std::string file) {
  std::ifstream inputFile(file, std::ios::binary);
  if (!inputFile) {
    throw std::runtime_error("Failed to open the file.");
  }

  size_t dotPos = file.rfind('.');
  std::string extension = file.substr(dotPos + 1);
  std::string filename = file.substr(0, dotPos);
  int extensionSize = extension.size();

  if (extension == "hcmp") {
    throw std::runtime_error("Invalid file type, hcmp is already compressed");
  }

  std::map<unsigned char, int> occurrences = get_occurrences(inputFile);
  std::cout << "Retrieved occurrences" << '\n';

  inputFile.clear();
  inputFile.seekg(0, std::ios::beg);

  std::vector<Node *> occurrenceNodes = get_occurrence_nodes(occurrences);
  std::cout << "Retrieved occurrence nodes" << '\n';

  Node *huffmanHead = create_huffman_tree(occurrenceNodes);
  std::cout << "Created Huffman tree" << '\n';

  // Pack the tree into a vector in preorder form to store in file
  std::vector<unsigned char> packedTree = get_tree_packet(huffmanHead);

  // Get the size of the tree to also store in file
  int treeSize = packedTree.size();

  // Create a look up table using the huffman tree
  std::map<unsigned char, std::string> table = createTable(huffmanHead);
  std::cout << "Created table" << '\n';

  // Delete head node, this will activate the destructor and free all child
  // nodes
  delete huffmanHead;
  std::cout << "Deleted Huffman head" << '\n';

  std::ofstream outputFile(filename + ".hcmp", std::ios::binary);
  if (outputFile) {
    int paddingNum = get_padding_amount(occurrences, table);
    outputFile.write(reinterpret_cast<const char *>(&paddingNum),
                     sizeof(paddingNum));
    outputFile.write(reinterpret_cast<const char *>(&extensionSize),
                     sizeof(extensionSize));
    outputFile.write((extension.c_str()), extensionSize);
    outputFile.write(reinterpret_cast<const char *>(&treeSize),
                     sizeof(treeSize));
    outputFile.write(reinterpret_cast<const char *>(packedTree.data()),
                     packedTree.size());
    std::string buffer;
    unsigned char byte;

    // While the file being compressed is not empty, read a single byte from the
    // file
    while (inputFile.read(reinterpret_cast<char *>(&byte), sizeof(byte))) {
      buffer += table[byte];

      // If the buffer has at least 8 characters, enough to form a single byte,
      // convert the buffer to a byte and write it to file
      while (buffer.size() >= 8) {
        std::string chunk = buffer.substr(0, 8);
        buffer = buffer.substr(8);
        unsigned char value =
            static_cast<unsigned char>(std::stoi(chunk, nullptr, 2));
        outputFile.write(reinterpret_cast<const char *>(&value), sizeof(value));
      }
    }

    // Handle remaining bits in the buffer
    if (!buffer.empty()) {
      // Since the remaining bits are less then 8, pad the with 0s
      buffer += std::string(paddingNum, '0');
      unsigned char value =
          static_cast<unsigned char>(std::stoi(buffer, nullptr, 2));
      outputFile.write(reinterpret_cast<const char *>(&value), sizeof(value));
    }

    outputFile.close();
    std::cout << "Data successfully compressed." << std::endl;
  } else {
    std::cout << "Failed to open the file." << std::endl;
  }
}
