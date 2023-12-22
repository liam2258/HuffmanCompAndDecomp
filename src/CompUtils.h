#ifndef COMP_UTILS_H
#define COMP_UTILS_H

#include "BitUtils.h"
#include "MapUtils.h"
#include "TreeUtils.h"
#include <cstring>


void decompressHelper(std::ofstream &outputFile, std::ifstream &inputFile,
                      Node *head, int remainder);
void decompress(Node *head, std::ofstream &outputFile, std::ifstream &inputFile,
                int remainder);
void decompressData(std::string file);
void compressData(std::string file);

#endif