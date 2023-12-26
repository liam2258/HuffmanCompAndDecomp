#include "CompUtils.h"

int main(int argc, char *argv[]) {

  if (argc < 2) {
    std::cout << "Please provide the file name as an argument." << std::endl;
    return 1;
  }

  std::string file = argv[1];

  // Check the mode (compression or decompression) based on the file extension
  std::string extension = file.substr(file.rfind('.') + 1);
  if (extension == "hcmp") {
    try {
      decompress_data(file);
    } catch (const std::exception &e) {
      std::cout << "Decompression failed: " << e.what() << std::endl;
    }
  } else {
    try {
      compress_data(file);
    } catch (const std::exception &e) {
      std::cout << "Compression failed: " << e.what() << std::endl;
    }
  }

  return 0;
}