#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "../../BitUtils.h"
#include "../../TreeUtils.h"



// Testing functions in BitUtils.h
TEST_CASE("Bit Manipulation: Testing BitUtils.h Functions") {
    SECTION("intToBytes() Tests:") {

        //Testing random integers
        int num1 = 256;
        std::vector<unsigned char> bytes1 = intToBytes(num1);

        REQUIRE(bytes1.size() == 4);
        REQUIRE(bytes1[0] == 0x00);
        REQUIRE(bytes1[1] == 0x00);
        REQUIRE(bytes1[2] == 0x01);
        REQUIRE(bytes1[3] == 0x00);

        int num2 = 8172;
        std::vector<unsigned char> bytes2 = intToBytes(num2);

        REQUIRE(bytes2.size() == 4);
        REQUIRE(bytes2[0] == 0x00);
        REQUIRE(bytes2[1] == 0x00);
        REQUIRE(bytes2[2] == 0x1F);
        REQUIRE(bytes2[3] == 0xEC);
        
        // Testing minimum sized integer 0
        int num3 = 0;
        std::vector<unsigned char> bytes3 = intToBytes(num3);

        REQUIRE(bytes3.size() == 4);
        REQUIRE(bytes3[0] == 0x00);
        REQUIRE(bytes3[1] == 0x00);
        REQUIRE(bytes3[2] == 0x00);
        REQUIRE(bytes3[3] == 0x00);

        // Testing maximum sized integer
        int num4 = 2147483647;
        std::vector<unsigned char> bytes4 = intToBytes(num4);

        REQUIRE(bytes4.size() == 4);
        REQUIRE(bytes4[0] == 0x7F);
        REQUIRE(bytes4[1] == 0xFF);
        REQUIRE(bytes4[2] == 0xFF);
        REQUIRE(bytes4[3] == 0xFF);

        // Testing negative integer, should throw invalid argument exception
        int num5 = -1;
        REQUIRE_THROWS_AS(intToBytes(num5), std::invalid_argument);
    }





    SECTION("byteToInt() Tests:") {
        // Testing random byte vectors
        std::vector<unsigned char> bytes1 {0x00, 0x00, 0x01, 0x00};
        REQUIRE(byteToInt(bytes1) == 256);

        std::vector<unsigned char> bytes2 = {0x00, 0x00, 0x1F, 0xEC};
        REQUIRE(byteToInt(bytes2) == 8172);

        // Testing for maximum sized integer
        std::vector<unsigned char> bytes3 = {0x7F, 0xFF, 0xFF, 0xFF};
        REQUIRE(byteToInt(bytes3) == 2147483647);

        // Testing integer of size 0
        std::vector<unsigned char> bytes4 = {0x00, 0x00, 0x00, 0x00};
        REQUIRE(byteToInt(bytes4) == 0);

        // Testing with invalid vector size, should throw invalid argument exception
        std::vector<unsigned char> bytes5 = {0x00};
        REQUIRE_THROWS_AS(byteToInt(bytes5), std::invalid_argument);
    }
}




// Helper function for getNodeList, this part of the code traverses the
// tree in post order form, adding each node to the list as it goes
void nodeListHelper(Node *given, std::vector<unsigned char> &list) {
    if (given) {
        list.push_back(given->value);
    } else {
        return;
    }
    nodeListHelper(given->left, list);
    nodeListHelper(given->right, list);
}

// Function to traverse a tree of nodes, gets every node value in post order form
// and returns them as a vector
std::vector<unsigned char> getNodeList(Node *given) {
    std::vector<unsigned char> list;
    nodeListHelper(given, list);
    return list;
}

// Helper function for getHuffmanValues, this part of the code traverses the
// tree in post order form, adding each node with no children to the list as it goes
void huffmanValuesHelper(Node *given, std::vector<unsigned char> &values) {
    if (given) {
        if (!given->left && !given->right) {
            values.push_back(given->value);
        }
    } else {
        return;
    }
    huffmanValuesHelper(given->left, values);
    huffmanValuesHelper(given->right, values);
}

// A function to help test the validity of huffman trees, function returns a vector of all the values in the huffman tree
// In post order form
std::vector<unsigned char> getHuffmanValues(Node *given) {
    std::vector<unsigned char> values;
    huffmanValuesHelper(given, values);
    return values;
}


// Helper function for getHuffmanFrequencies, this part of the code actually traverse the
// tree in post order form, adding each node to the list as it goes
void huffmanFrequenciesHelper(Node *given, std::vector<int> &frequencies) {
    if (given) {
        frequencies.push_back(given->frequency);
    } else {
        return;
    }
    huffmanFrequenciesHelper(given->left, frequencies);
    huffmanFrequenciesHelper(given->right, frequencies);
}

// A function to help test the validity of huffman trees, function returns a vector of all the frequencies in the huffman tree
// In post order form
std::vector<int> getHuffmanFrequencies(Node *given) {
    std::vector<int> frequencies;
    huffmanFrequenciesHelper(given, frequencies);
    return frequencies;
}


// Testing functions in TreeUtils.h
TEST_CASE("Tree Manipulation: Testing TreeUtils.h Functions") {
    SECTION("getTreePacket() Tests:") {

        // Testing when given tree is empty

        Node* root = nullptr;
        REQUIRE_THROWS_AS(getTreePacket(root), std::invalid_argument);



        //Testing a tree with a single node
        
        // Constructing single node for testing
        Node* singleNode = new Node('A', 2);

        // Constructing expected packet
        std::vector<unsigned char> expectedPacket = {
            'A', 0x00, 0x00, 0x00, 0x02, 'A',
        };

        REQUIRE(getTreePacket(singleNode) == expectedPacket);

        delete singleNode;



        // Testing Random valid tree 1

        // Construct a random valid tree for testing
        Node* testNode1 = new Node('A', 2);
        Node* testNode2 = new Node('B', 3);
        Node* testNode3 = new Node('C', 4);
        Node* testNode4 = new Node('D', 5);
        Node* testNode5 = new Node('E', 6);

        testNode1->left = testNode2;
        testNode1->right = testNode3;
        testNode3->left = testNode4;
        testNode4->right = testNode5;

        // Construct expected packet response
        std::vector<unsigned char> expectedPacket2 = {
            'A', 0x00, 0x00, 0x00, 0x02, 'D',
            'B', 0x00, 0x00, 0x00, 0x03, 'A',
            'C', 0x00, 0x00, 0x00, 0x04, 'C',
            'D', 0x00, 0x00, 0x00, 0x05, 'B',
            'E', 0x00, 0x00, 0x00, 0x06, 'A',
        };

        REQUIRE(getTreePacket(testNode1) == expectedPacket2);

        delete testNode1;


        // Testing Random valid tree 2

        // Construct a random valid tree for testing
        Node* test2Node1 = new Node('A', 2);
        Node* test2Node2 = new Node('B', 3);
        Node* test2Node3 = new Node('C', 4);
        Node* test2Node4 = new Node('D', 5);
        Node* test2Node5 = new Node('E', 6);
        Node* test2Node6 = new Node('F', 7);
        Node* test2Node7 = new Node('G', 8);

        test2Node1->left = test2Node2;
        test2Node1->right = test2Node3;
        test2Node2->left = test2Node4;
        test2Node2->right = test2Node5;
        test2Node3->left = test2Node6;
        test2Node3->right = test2Node7;

        // Construct expected packet response
        std::vector<unsigned char> expectedPacket3 = {
            'A', 0x00, 0x00, 0x00, 0x02, 'D',
            'B', 0x00, 0x00, 0x00, 0x03, 'D',
            'D', 0x00, 0x00, 0x00, 0x05, 'A',
            'E', 0x00, 0x00, 0x00, 0x06, 'A',
            'C', 0x00, 0x00, 0x00, 0x04, 'D',
            'F', 0x00, 0x00, 0x00, 0x07, 'A',
            'G', 0x00, 0x00, 0x00, 0x08, 'A',
        };

        REQUIRE(getTreePacket(test2Node1) == expectedPacket3);

        delete test2Node1;
    }





    SECTION("treeReconstructor() Tests:") {

        // Constructing an empty packet
        std::vector<unsigned char> nodePacket = {};

        // Testing empty packet
        REQUIRE_THROWS_AS(treeReconstructor(nodePacket), std::invalid_argument);



        // Constructing a packet of invalid size
        std::vector<unsigned char> nodePacket2 = {'A'};

        // Testing invalidly sized packet
        REQUIRE_THROWS_AS(treeReconstructor(nodePacket2), std::invalid_argument);



        // Constructing random valid packet
        std::vector<unsigned char> nodePacket3 = {
            'A', 0x00, 0x00, 0x00, 0x02, 'A',
        };

        // Getting the function response
        Node* response3 = treeReconstructor(nodePacket3);

        // Testing random valid packet
        // Test expected tree size, should be 1
        std::vector<unsigned char> tree3 = getNodeList(response3);
        REQUIRE(tree3.size() == 1);
        // Testing tree for correct content
        REQUIRE(tree3 == std::vector<unsigned char> {'A'});

        delete response3;



        // Constructing random valid packet2
        std::vector<unsigned char> nodePacket4 = {
            'A', 0x00, 0x00, 0x00, 0x02, 'D',
            'B', 0x00, 0x00, 0x00, 0x03, 'A',
            'C', 0x00, 0x00, 0x00, 0x04, 'C',
            'D', 0x00, 0x00, 0x00, 0x05, 'B',
            'E', 0x00, 0x00, 0x00, 0x06, 'A',
        };

        // Getting the function response
        Node* response4 = treeReconstructor(nodePacket4);

        // Testing random valid packet
        // Test expected tree size, should be 5
        std::vector<unsigned char> tree4 = getNodeList(response4);
        REQUIRE(tree4.size() == 5);
        // Testing tree2 for correct content
        REQUIRE(tree4 == std::vector<unsigned char> {'A', 'B', 'C', 'D', 'E'});

        delete response4;



        // Constructing random valid packet3
        std::vector<unsigned char> nodePacket5 = {
            'A', 0x00, 0x00, 0x00, 0x02, 'D',
            'B', 0x00, 0x00, 0x00, 0x03, 'D',
            'D', 0x00, 0x00, 0x00, 0x05, 'A',
            'E', 0x00, 0x00, 0x00, 0x06, 'A',
            'C', 0x00, 0x00, 0x00, 0x04, 'D',
            'F', 0x00, 0x00, 0x00, 0x07, 'A',
            'G', 0x00, 0x00, 0x00, 0x08, 'A',
        };

        // Getting the function response
        Node* response5 = treeReconstructor(nodePacket5);

        // Testing random valid packet
        // Test expected tree size, should be 5
        std::vector<unsigned char> tree5 = getNodeList(response5);
        REQUIRE(tree5.size() == 7);
        // Testing tree2 for correct content
        REQUIRE(tree5 == std::vector<unsigned char> {'A', 'B', 'D', 'E', 'C', 'F', 'G'});

        delete response5;
    }



    SECTION("createHuffmanTree() Tests:") {
        // Create an empty vector for testing
        std::vector<Node *> nodeVector1;

        // Test empty vector, should return error
        REQUIRE_THROWS_AS(createHuffmanTree(nodeVector1), std::invalid_argument);

        // Create a small valid vector for testing
        std::vector<Node *> nodeVector2 {new Node('A', 12)};

        Node *huffmanTree2 = createHuffmanTree(nodeVector2);

        
        REQUIRE(getHuffmanValues(huffmanTree2) == std::vector<unsigned char> {'A'});
        REQUIRE(getHuffmanFrequencies(huffmanTree2) == std::vector<int> {0, 12});

        delete huffmanTree2;



        // Create a second random valid vector for testing
        std::vector<Node *> nodeVector3 {new Node('A', 12), new Node('B', 3), new Node('C', 22), new Node('D', 10)};

        Node *huffmanTree3 = createHuffmanTree(nodeVector3);

        REQUIRE(getHuffmanValues(huffmanTree3) == std::vector<unsigned char> {'C', 'A', 'B', 'D'});
        REQUIRE(getHuffmanFrequencies(huffmanTree3) == std::vector<int> {47, 22, 25, 12, 13, 3, 10});

        delete huffmanTree3;



        // Create a third random valid vector for testing
        std::vector<Node *> nodeVector4 {new Node('A', 12), new Node('B', 5), new Node('C', 22), new Node('D', 10),
                                        new Node('E', 15), new Node('F', 30), new Node('G', 29)};

        Node *huffmanTree4 = createHuffmanTree(nodeVector4);

        REQUIRE(getHuffmanValues(huffmanTree4) == std::vector<unsigned char> {'A', 'E', 'G', 'F', 'B', 'D', 'C'});
        REQUIRE(getHuffmanFrequencies(huffmanTree4) == std::vector<int> {123, 56, 27, 12, 15, 29, 67, 30, 37, 15, 5, 10, 22});

        delete huffmanTree4;
    }
}