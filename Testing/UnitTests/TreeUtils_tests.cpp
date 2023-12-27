#include "../../src/TreeUtils.h"
#include "catch.hpp"


// Helper function for get_node_list, this part of the code traverses the
// tree in post order form, adding each node to the list as it goes
void node_values_helper(Node *given, std::vector<unsigned char> &list) {
  if (given) {
    list.push_back(given->value);
  } else {
    return;
  }
  node_values_helper(given->left, list);
  node_values_helper(given->right, list);
}

// Function to traverse a tree of nodes, gets every node value in post order
// form and returns them as a vector
std::vector<unsigned char> get_node_list(Node *given) {
  std::vector<unsigned char> list;
  node_values_helper(given, list);
  return list;
}

// Helper function for get_huffman_values, this part of the code traverses the
// tree in post order form, adding each node with no children to the list as it
// goes
void huffman_values_helper(Node *given, std::vector<unsigned char> &values) {
  if (given) {
    if (!given->left && !given->right) {
      values.push_back(given->value);
    }
  } else {
    return;
  }
  huffman_values_helper(given->left, values);
  huffman_values_helper(given->right, values);
}

// A function to help test the validity of huffman trees, function returns a
// vector of all the values in the huffman tree In post order form
std::vector<unsigned char> get_huffman_values(Node *given) {
  std::vector<unsigned char> values;
  huffman_values_helper(given, values);
  return values;
}

// Helper function for getHuffmanFrequencies, this part of the code actually
// traverse the tree in post order form, adding each node to the list as it goes
void huffman_frequencies_helper(Node *given, std::vector<int> &frequencies) {
  if (given) {
    frequencies.push_back(given->frequency);
  } else {
    return;
  }
  huffman_frequencies_helper(given->left, frequencies);
  huffman_frequencies_helper(given->right, frequencies);
}

// A function to help test the validity of huffman trees, function returns a
// vector of all the frequencies in the huffman tree In post order form
std::vector<int> get_huffman_frequencies(Node *given) {
  std::vector<int> frequencies;
  huffman_frequencies_helper(given, frequencies);
  return frequencies;
}

// Testing functions in TreeUtils.h
TEST_CASE("Tree Manipulation: Testing TreeUtils.h Functions") {
  SECTION("get_tree_packet() Tests:") {

    // Testing when given tree is empty

    Node *root = nullptr;
    REQUIRE_THROWS_AS(get_tree_packet(root), std::invalid_argument);

    // Testing a tree with a single node

    // Constructing single node for testing
    Node *singleNode = new Node('A', 2);

    // Constructing expected packet
    std::vector<unsigned char> expectedPacket = {
        'A', 0x00, 0x00, 0x00, 0x02, 'A',
    };

    REQUIRE(get_tree_packet(singleNode) == expectedPacket);

    delete singleNode;

    // Testing Random valid tree 1

    // Construct a random valid tree for testing
    Node *testNode1 = new Node('A', 2);
    Node *testNode2 = new Node('B', 3);
    Node *testNode3 = new Node('C', 4);
    Node *testNode4 = new Node('D', 5);
    Node *testNode5 = new Node('E', 6);

    testNode1->left = testNode2;
    testNode1->right = testNode3;
    testNode3->left = testNode4;
    testNode4->right = testNode5;

    // Construct expected packet response
    std::vector<unsigned char> expectedPacket2 = {
        'A',  0x00, 0x00, 0x00, 0x02, 'D',  'B',  0x00, 0x00, 0x00,
        0x03, 'A',  'C',  0x00, 0x00, 0x00, 0x04, 'C',  'D',  0x00,
        0x00, 0x00, 0x05, 'B',  'E',  0x00, 0x00, 0x00, 0x06, 'A',
    };

    REQUIRE(get_tree_packet(testNode1) == expectedPacket2);

    delete testNode1;

    // Testing Random valid tree 2

    // Construct a random valid tree for testing
    Node *test2Node1 = new Node('A', 2);
    Node *test2Node2 = new Node('B', 3);
    Node *test2Node3 = new Node('C', 4);
    Node *test2Node4 = new Node('D', 5);
    Node *test2Node5 = new Node('E', 6);
    Node *test2Node6 = new Node('F', 7);
    Node *test2Node7 = new Node('G', 8);

    test2Node1->left = test2Node2;
    test2Node1->right = test2Node3;
    test2Node2->left = test2Node4;
    test2Node2->right = test2Node5;
    test2Node3->left = test2Node6;
    test2Node3->right = test2Node7;

    // Construct expected packet response
    std::vector<unsigned char> expectedPacket3 = {
        'A',  0x00, 0x00, 0x00, 0x02, 'D',  'B',  0x00, 0x00, 0x00, 0x03,
        'D',  'D',  0x00, 0x00, 0x00, 0x05, 'A',  'E',  0x00, 0x00, 0x00,
        0x06, 'A',  'C',  0x00, 0x00, 0x00, 0x04, 'D',  'F',  0x00, 0x00,
        0x00, 0x07, 'A',  'G',  0x00, 0x00, 0x00, 0x08, 'A',
    };

    REQUIRE(get_tree_packet(test2Node1) == expectedPacket3);

    delete test2Node1;
  }

  SECTION("tree_reconstructor() Tests:") {

    // Constructing an empty packet
    std::vector<unsigned char> nodePacket = {};

    // Testing empty packet
    REQUIRE_THROWS_AS(tree_reconstructor(nodePacket), std::invalid_argument);

    // Constructing a packet of invalid size
    std::vector<unsigned char> nodePacket2 = {'A'};

    // Testing invalidly sized packet
    REQUIRE_THROWS_AS(tree_reconstructor(nodePacket2), std::invalid_argument);

    // Constructing random valid packet
    std::vector<unsigned char> nodePacket3 = {
        'A', 0x00, 0x00, 0x00, 0x02, 'A',
    };

    // Getting the function response
    Node *response3 = tree_reconstructor(nodePacket3);

    // Testing random valid packet
    // Test expected tree size, should be 1
    std::vector<unsigned char> tree3 = get_node_list(response3);
    REQUIRE(tree3.size() == 1);
    // Testing tree for correct content
    REQUIRE(tree3 == std::vector<unsigned char>{'A'});

    delete response3;

    // Constructing random valid packet2
    std::vector<unsigned char> nodePacket4 = {
        'A',  0x00, 0x00, 0x00, 0x02, 'D',  'B',  0x00, 0x00, 0x00,
        0x03, 'A',  'C',  0x00, 0x00, 0x00, 0x04, 'C',  'D',  0x00,
        0x00, 0x00, 0x05, 'B',  'E',  0x00, 0x00, 0x00, 0x06, 'A',
    };

    // Getting the function response
    Node *response4 = tree_reconstructor(nodePacket4);

    // Testing random valid packet
    // Test expected tree size, should be 5
    std::vector<unsigned char> tree4 = get_node_list(response4);
    REQUIRE(tree4.size() == 5);
    // Testing tree2 for correct content
    REQUIRE(tree4 == std::vector<unsigned char>{'A', 'B', 'C', 'D', 'E'});

    delete response4;

    // Constructing random valid packet3
    std::vector<unsigned char> nodePacket5 = {
        'A',  0x00, 0x00, 0x00, 0x02, 'D',  'B',  0x00, 0x00, 0x00, 0x03,
        'D',  'D',  0x00, 0x00, 0x00, 0x05, 'A',  'E',  0x00, 0x00, 0x00,
        0x06, 'A',  'C',  0x00, 0x00, 0x00, 0x04, 'D',  'F',  0x00, 0x00,
        0x00, 0x07, 'A',  'G',  0x00, 0x00, 0x00, 0x08, 'A',
    };

    // Getting the function response
    Node *response5 = tree_reconstructor(nodePacket5);

    // Testing random valid packet
    // Test expected tree size, should be 5
    std::vector<unsigned char> tree5 = get_node_list(response5);
    REQUIRE(tree5.size() == 7);
    // Testing tree2 for correct content
    REQUIRE(tree5 ==
            std::vector<unsigned char>{'A', 'B', 'D', 'E', 'C', 'F', 'G'});

    delete response5;
  }

  SECTION("create_huffman_tree() Tests:") {
    // Create an empty vector for testing
    std::vector<Node *> nodeVector1;

    // Test empty vector, should return error
    REQUIRE_THROWS_AS(create_huffman_tree(nodeVector1), std::invalid_argument);

    // Create a small valid vector for testing
    std::vector<Node *> nodeVector2{new Node('A', 12)};

    Node *huffmanTree2 = create_huffman_tree(nodeVector2);

    REQUIRE(get_huffman_values(huffmanTree2) ==
            std::vector<unsigned char>{'A'});
    REQUIRE(get_huffman_frequencies(huffmanTree2) == std::vector<int>{0, 12});

    delete huffmanTree2;

    // Create a second random valid vector for testing
    std::vector<Node *> nodeVector3{new Node('A', 12), new Node('B', 3),
                                    new Node('C', 22), new Node('D', 10)};

    Node *huffmanTree3 = create_huffman_tree(nodeVector3);

    REQUIRE(get_huffman_values(huffmanTree3) ==
            std::vector<unsigned char>{'C', 'A', 'B', 'D'});
    REQUIRE(get_huffman_frequencies(huffmanTree3) ==
            std::vector<int>{47, 22, 25, 12, 13, 3, 10});

    delete huffmanTree3;

    // Create a third random valid vector for testing
    std::vector<Node *> nodeVector4{new Node('A', 12), new Node('B', 5),
                                    new Node('C', 22), new Node('D', 10),
                                    new Node('E', 15), new Node('F', 30),
                                    new Node('G', 29)};

    Node *huffmanTree4 = create_huffman_tree(nodeVector4);

    REQUIRE(get_huffman_values(huffmanTree4) ==
            std::vector<unsigned char>{'A', 'E', 'G', 'F', 'B', 'D', 'C'});
    REQUIRE(
        get_huffman_frequencies(huffmanTree4) ==
        std::vector<int>{123, 56, 27, 12, 15, 29, 67, 30, 37, 15, 5, 10, 22});

    delete huffmanTree4;
  }
}