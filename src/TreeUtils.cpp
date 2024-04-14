#include "TreeUtils.h"

const int MAX_INT = 2147483647;

/**
 * Traverses a Huffman tree in pre-order form and packs the tree into a vector
 * of bytes.
 *
 * This function traverses the tree in pre-order form (root, left, right). For
 * each node, it adds the node's value and frequency to the vector. The
 * frequency is converted into a vector of bytes before being added. After
 * adding the value and frequency, the function adds a child flag to the vector.
 * This flag indicates whether the node has a left child, a right child, both,
 * or none.
 *
 * @param head The root of the Huffman tree to be packed.
 * @param tree The vector where the packed tree will be stored.
 */
void pre_order_packing(Node *head, std::vector<unsigned char> &tree) {
  if (head == nullptr) {
    return;
  }
  // First add the node value
  tree.push_back(head->value);

  // Second add the frequency

  // Convert the frequency integer into a vector of unsigned char
  // Each item in the vector is once of 4 bytes that make up an integer
  std::vector<unsigned char> frequency = int_to_bytes(head->frequency);

  // Add the frequency vector to the tree vector
  tree.insert(tree.end(), frequency.begin(), frequency.end());

  // Lasly based on if the node has left or right or no children add an
  // identifier
  if (head->left == nullptr && head->right == nullptr) {
    tree.push_back('A');
  } else if (head->left == nullptr) {
    tree.push_back('B');
  } else if (head->right == nullptr) {
    tree.push_back('C');
  } else {
    tree.push_back('D');
  }
  pre_order_packing(head->left, tree);
  pre_order_packing(head->right, tree);
}

/**
 * Creates a packet of bytes representing a Huffman tree.
 *
 * This function takes the root of a Huffman tree and packs it into a vector of
 * bytes using the pre_order_packing function. The packed tree can be stored in
 * a file and used to rebuild the tree during decompression.
 *
 * @param head The root of the Huffman tree to be packed.
 * @return A vector of bytes representing the Huffman tree.
 * @throws std::invalid_argument If the tree is empty (i.e., the head is null).
 */
std::vector<unsigned char> get_tree_packet(Node *head) {
  if (head == nullptr) {
    throw std::invalid_argument("Tree is empty.");
  }

  std::vector<unsigned char> tree;

  pre_order_packing(head, tree);

  return tree;
}

/**
 * Links nodes together to reconstruct a Huffman tree.
 *
 * This function takes a node and a vector of nodes and flags. The flags
 * indicate whether a node has a left child, a right child, both, or none. Based
 * on the flag, the function links the node to its children and recursively
 * calls itself on the children. The tracker variable keeps track of the current
 * index in the nodes and flags vectors during the recursive calls.
 *
 * @param current The current node being processed.
 * @param nodes The vector of nodes to be linked together.
 * @param flags The vector of flags indicating the children of each node.
 * @param tracker The current index in the nodes and flags vectors.
 */
void link_nodes(Node *current, std::vector<Node *> &nodes,
                std::vector<unsigned char> &flags, int &tracker) {
  switch (flags[tracker]) {
  case 'A': // In the case where a has no children
    return;
  case 'B': // In the case where a node only has a right child
    tracker++;
    current->right = nodes[tracker];
    link_nodes(nodes[tracker], nodes, flags, tracker);
    break;
  case 'C': // In the case where a node only has a left child
    tracker++;
    current->left = nodes[tracker];
    link_nodes(nodes[tracker], nodes, flags, tracker);
    break;
  case 'D': // In the case where a node has a left and right child
    tracker++;
    current->left = nodes[tracker];
    link_nodes(nodes[tracker], nodes, flags, tracker);
    tracker++;
    current->right = nodes[tracker];
    link_nodes(nodes[tracker], nodes, flags, tracker);
    break;
  }
}

/**
 * Reconstructs a Huffman tree from a packet of bytes.
 *
 * This function takes a vector of bytes representing a Huffman tree and
 * reconstructs the tree. The vector is expected to contain a series of 6-byte
 * bundles, where each bundle represents a node. The first byte of each bundle
 * is the node's value, the next four bytes are the node's frequency, and the
 * last byte is a flag indicating the node's children.
 *
 * The function first checks if the packet size is valid. If not, it throws an
 * invalid_argument exception. It then extracts the nodes and flags from the
 * packet and uses the link_nodes function to link the nodes together and
 * reconstruct the tree.
 *
 * @param treePacket The vector of bytes representing the Huffman tree.
 * @return The root of the reconstructed Huffman tree.
 * @throws std::invalid_argument If the packet size is invalid.
 */
Node *tree_reconstructor(std::vector<unsigned char> treePacket) {
  std::vector<Node *> nodes;
  std::vector<unsigned char> flags;

  // Check if the packet has a valid size
  if (treePacket.size() < 6 || treePacket.size() % 6 != 0) {
    throw std::invalid_argument("Invalid packet size");
  }

  // Extract the nodes and the flags (Every bundle has 6 bytes where the 1st
  // byte is the node value, the 2-5th are the frequency, and the 6th is a flag)
  for (std::vector<unsigned char>::size_type i = 0; i <= treePacket.size() - 6; i += 6) {
    // Grab bytes 2-5 which make up the frequency integer value and convert it
    // to an integer
    std::vector<unsigned char> frequencyBytes{
        treePacket[i + 1], treePacket[i + 2], treePacket[i + 3],
        treePacket[i + 4]};
    int frequency = byte_to_int(frequencyBytes);

    // Create a new node using the value the node is representing
    // (treePacket[i]) and the frequency integer we just created Then add the
    // node to the nodes vector
    nodes.push_back(new Node(treePacket[i], frequency));

    // Retrieve the flag, which identifies how many children each node has, and
    // add it to the flag vector
    flags.push_back(treePacket[i + 5]);
  }

  // Tracker used to keep track of index during recursive function calling
  int tracker = 0;

  // Use the created nodes and flags vector to link the nodes together into a
  // tree
  link_nodes(nodes[0], nodes, flags, tracker);

  return nodes[0];
}


/**
 * Constructs a Huffman tree from a vector of nodes.
 *
 * This function takes a vector of nodes and recursively constructs a Huffman
 * tree. It finds the two nodes with the lowest frequencies and creates a new
 * parent node that combines their frequencies. It then removes the two child
 * nodes from the vector and adds the parent node. This process continues until
 * only one node is left in the vector, which is the root of the Huffman tree.
 *
 * @param nodes The vector of nodes to be used to construct the Huffman tree.
 */
void huffman_constructor(std::vector<Node *> &nodes) {
  int lowestFreq1 = MAX_INT;
  int lowestFreq2 = MAX_INT;
  std::vector<Node*>::size_type minIndex1 = -1; // Change type to size_type
  std::vector<Node*>::size_type minIndex2 = -1; // Change type to size_type

  // Get the 2 lowest frequency nodes
  for (std::vector<Node*>::size_type i = 0; i < nodes.size(); ++i) { // Change type to size_type
    if (nodes[i]->frequency < lowestFreq1) {
      lowestFreq2 = lowestFreq1;
      minIndex2 = minIndex1;

      lowestFreq1 = nodes[i]->frequency;
      minIndex1 = i;
    } else if (nodes[i]->frequency < lowestFreq2) {
      lowestFreq2 = nodes[i]->frequency;
      minIndex2 = i;
    }
  }

  Node *minNode1 = nodes[minIndex1];
  Node *minNode2 = nodes[minIndex2];

  // Create a new node that combines the two frequencies of the child nodes,
  // assigning the smaller node as it's left child and the larger node as it's
  // right child
  int newFreq = minNode1->frequency + minNode2->frequency;
  Node *newNode = new Node('Z', newFreq);
  newNode->left = minNode1;
  newNode->right = minNode2;

  // Check which index location the min value is located, if minIndex1 is larger
  // delete it first if not delete it second.
  // This is done to keep the indexes from changing after the first erasure
  if (minIndex1 > minIndex2) {
    nodes.erase(nodes.begin() + minIndex1);
    nodes.erase(nodes.begin() + minIndex2);
  } else {
    nodes.erase(nodes.begin() + minIndex2);
    nodes.erase(nodes.begin() + minIndex1);
  }
  nodes.push_back(newNode);
  if (nodes.size() > 2) {
    huffman_constructor(nodes);
  }
}

/**
 * Creates a Huffman tree from a vector of nodes.
 *
 * This function takes a vector of nodes and constructs a Huffman tree.
 * If the vector is empty, it throws an invalid_argument exception.
 * If the vector contains only one node, it creates a new parent node with a
 * frequency of 0 and the single node as its left child. If the vector contains
 * more than one node, it calls the huffman_constructor function to construct
 * the tree. Once there are only two nodes left, it creates a final parent node
 * and makes the less frequently used node the left child and the more
 * frequently used node the right child.
 *
 * @param nodes The vector of nodes to be used to construct the Huffman tree.
 * @return The root of the Huffman tree.
 * @throws std::invalid_argument If the vector is empty.
 */
Node *create_huffman_tree(std::vector<Node *> &nodes) {
  if (nodes.size() < 1) {
    throw std::invalid_argument("Empty vector");
  }

  if (nodes.size() < 2) {
    Node *temp = new Node();
    temp->frequency = 0;
    temp->left = nodes[0];
    return temp;
  }
  huffman_constructor(nodes);

  int frequency = nodes[0]->frequency + nodes[1]->frequency;
  Node *head = new Node(0, frequency);
  if (nodes[0]->frequency < nodes[1]->frequency) {
    head->left = nodes[0];
    head->right = nodes[1];
  } else {
    head->left = nodes[1];
    head->right = nodes[0];
  }
  return head;
}

/**
 * Recursively traverses a Huffman tree in pre-order form and creates a map of
 * Huffman codes.
 *
 * This function takes the root of a Huffman tree, a path string, and a
 * reference to a map. It traverses the tree in pre-order, appending '0' to the
 * path string when it goes left and '1' when it goes right. When it finds a
 * leaf node (a node with no children), it adds the node's value and the path
 * string to the map. The map then represents the Huffman codes for each byte
 * value in the tree.
 *
 * @param head The root of the Huffman tree.
 * @param path The current path string.
 * @param table The map where the Huffman codes will be stored.
 */
void find_tree_path(Node *head, std::string path,
                    std::map<unsigned char, std::string> &table) {
  if (head == nullptr) {
    return;
  }

  if (head->left == nullptr && head->right == nullptr) {
    table[head->value] = path;
    return;
  }
  find_tree_path(head->left, path + '0', table);
  find_tree_path(head->right, path + '1', table);
}

/**
 * Creates a look-up table from a Huffman tree.
 *
 * This function takes the root of a Huffman tree and creates a look-up table.
 * The table is a map where the key is a node value and the value is the binary
 * path to reach that node in the tree. The function uses the find_tree_path
 * helper function to traverse the tree and fill the table.
 *
 * @param huffmanHead The root of the Huffman tree.
 * @return A map representing the look-up table.
 */
std::map<unsigned char, std::string> createTable(Node *huffmanHead) {
  std::map<unsigned char, std::string> table;
  find_tree_path(huffmanHead, "", table);
  return table;
}