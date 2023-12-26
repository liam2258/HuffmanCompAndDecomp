#ifndef COMP_UTILS_H
#define COMP_UTILS_H

#include "BitUtils.h"
#include "MapUtils.h"
#include "TreeUtils.h"
#include <cstring>


void decompress_helper(std::ofstream &outputFile, std::ifstream &inputFile,
                      Node *head, int remainder);
void decompress(Node *head, std::ofstream &outputFile, std::ifstream &inputFile,
                int remainder);
void decompress_data(std::string file);
void compress_data(std::string file);

#endif