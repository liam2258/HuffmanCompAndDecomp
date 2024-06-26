# C++ Huffman Compressor and Decompressor

A file compressor and decompressor for Linux, written in C++ that utilizes Huffman coding. The program takes a file and outputs a compressed version of it as a .hcmp file (Short for Huffman compression!). These hcmp files can then be decompressed into their original state.

## Table of Contents

- [Deployment](#deployment)
- [Running Tests](#running-tests)
- [Methods](#methods)
- [Packet Structure](#packet-structure)
- [Compression Examples](#compression-examples)

## Deployment

This project includes a Makefile and can be compiled with the following command from the root directory:

```bash
  make
```

Once compiled, the executable will take a filename as an argument in the command line. It will automtically detect if it is to perform compression or decompression based on the filename, hcmp files will be decompressed and all other files will be compressed. It can be executed using the example below:

```bash
  ./main my_file.txt
```

This will produce a compressed hcmp version of the file in the same directory as the executable.

## Running Tests

This program utilizes Catch2 for unit testing and the header is included in the repository. Running the tests can be done similarly to compliation using a make command:

```bash
  make test
```

The tests will run automatically but for more detail you can re-run the exectuable using:

```bash
  ./test_main -s
```

Additionally this program also utilizes Pytest for end-to-end testing. This includes testing the compilation of the program and testing its compression and decompression features on two files. From the root directory they can be run with the following commands:

```bash
  cd Testing/E2ETests
  pytest tests.py
```

These tests can similarly be expanded for more detail:

```bash
  pytest tests.py -v
```

If you want to skip the compilation of the program you can use the following flag:

```bash
  pytest tests.py -k "not test_compilation"
```

## Methods

The program's compression method is to read the given file twice, once to create a Huffman tree for compression and again to actually compress the file. It first reads over the file, keeping track of each character and how often it appears. This information is then used to create a binary Huffman tree. With the Huffman tree created we can then make a path hash, where every character in the file is given a specific representation in binary. Finally it commits this information to the hcmp file using a packet like structure before filling the file with the binary translation of the original file.

For decompression the process is very similar. The packet structure embedded in each hcmp file contains the Huffman tree used to create it. This data is parsed and used to recreate the tree and then the compression process is reversed. The hcmp file is read bit by bit and every time a series of bits matches a character in the Huffman tree it is added to the decompressed file until the entire file is restored.

## Packet Structure

Packets that head each hcmp file are constructed in this format:

| Field             | Size         |
| ----------------- | ------------ |
| Remainder         | 4 bytes      |
| Extension Size    | 4 bytes      |
| Extension         | Varying size |
| Huffman Tree Size | 4 bytes      |
| Huffman Tree      | Varying size |

Remainder: How many useless bits are added to the end of the file to make a complete byte

Extension Size: How many characters the original files extension is

Extension: The file extension characters

Huffman Tree Size: How many bytes the Huffman tree occupies

Huffman Tree: The Huffman Tree Data

## Compression Examples

| File                    | Before | After   |
| ----------------------- | ------ | ------- |
| alice_in_wonderland.txt | 146 KB | 84 KB   |
| kjv.txt                 | 4.2 MB | 2.4 MB  |
| rockyou.txt             | 133 MB | 86.7 MB |
