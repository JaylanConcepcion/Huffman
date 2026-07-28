#include "Huffman.h"

#include <fstream>
#include <iostream>
#include <unordered_map>
#include <queue>
#include "Node.h"
#include "Storage/Storage.h"


Huffman::Huffman() {
    root = nullptr;
}


Huffman::~Huffman() {
    deleteTree(root);
}


void Huffman::deleteTree(Node* node) {
    if (!node) return;
    deleteTree(node->zero);
    deleteTree(node->one);
    delete node;
}

std::string Huffman::inFiles(const std::string& filename) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return "";
    }

    std::string content;
    std::string line;
    while (std::getline(inputFile, line)) {
        content += line + "\n";
    }

    if (content.empty()) {
        std::cerr << "File is empty: " << filename << std::endl;
    }
    return content;
}


// Counts how many times each allowed character appears in the text
void Huffman::frequencyTable(const std::string& text, std::unordered_map<char, int>& freqTable) {
    for (size_t i = 0; i < text.size(); ++i) {
        char c = text[i]; // Get character
        if ((c >= 32 && c <= 126) || c == '\n' || c == '\t') {
            freqTable[c]++; // Count allowed characters
        }
    }
}
// Builds the Huffman tree using a min-heap priority queue
void Huffman::buildTree(const std::unordered_map<char, int>& freqTable) {
    // Priority queue using compareWeights functor (
    std::priority_queue<Node*, std::vector<Node*>, compareWeights> pq;

    // Create a node for each character and insert into the queue
    std::unordered_map<char, int>::const_iterator it;
    for (it = freqTable.begin(); it != freqTable.end(); ++it) {
        char character = it->first;
        int frequency = it->second;
        pq.push(new Node(character, frequency)); // Put each character node into the queue
    }

    // Combine nodes until one root node remains
    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();  // Get first smallest
        Node* right = pq.top(); pq.pop(); // Get second smallest
        Node* parent = new Node('\0', left->weight + right->weight, left, right); // Combine them
        pq.push(parent); // Add new node to queue
    }

    root = pq.top(); // Root of Huffman tree
}

// Creates binary codes for each character in the tree
void Huffman::making_codes(Node* node, const std::string& code, std::unordered_map<char, std::string>& huffmanCodes) {
    if (!node) return; // Base case
    if (!node->zero && !node->one) {
        huffmanCodes[node->letter] = code; // Save code if it's a leaf
    }
    making_codes(node->zero, code + "0", huffmanCodes); // Go left
    making_codes(node->one, code + "1", huffmanCodes); // Go right
}

// Turns the Huffman tree into a string so it can be saved to a file
std::string Huffman::encodeTree(Node* node) {
    if (!node) return "0";
    if (!node->zero && !node->one) return "1" + std::string(1, node->letter); // Leaf node
    return "0" + encodeTree(node->zero) + encodeTree(node->one); // Internal node
}

// Rebuilds the Huffman tree from its encoded string format
Node* Huffman::decodeTree(const std::string& data, size_t& index) {
    if (index >= data.size()) return nullptr; // Check bounds
    char current = data[index++]; // Read one character
    if (current == '1') {
        return new Node(data[index++], 0); // Create leaf node
    }
    Node* left = decodeTree(data, index); // Rebuild left
    Node* right = decodeTree(data, index); // Rebuild right
    return new Node('\0', 0, left, right); // Return new parent
}

// Sets the root of the tree by decoding the saved tree header
void Huffman::reconstruct_tree(const std::string& header) {
    size_t index = 0;
    root = decodeTree(header, index);
}

// Creates a map from binary code strings to characters
void Huffman::ReverseCodes(Node* node, const std::string& code, std::unordered_map<std::string, char>& code_Char) {
    if (!node) return; // Base case
    if (!node->zero && !node->one) {
        code_Char[code] = node->letter; // Map code to character
    }
    ReverseCodes(node->zero, code + "0", code_Char); // Go left
    ReverseCodes(node->one, code + "1", code_Char); // Go right
}

// Compress
void Huffman::compress(const std::string& inputFile, const std::string& outputFile) {
    std::string content = inFiles(inputFile);
    if (content.empty()) return;

    std::unordered_map<char, int> freqTable;
    frequencyTable(content, freqTable); // Count characters

    buildTree(freqTable); // Build Huffman tree

    std::unordered_map<char, std::string> huffmanCodes;
    making_codes(root, "", huffmanCodes); // Create codes

    std::string header = encodeTree(root) + "\n"; // Store tree as string

    Storage storage;
    if (!storage.open(outputFile, "write")) {
        std::cerr << "Failed to open output file: " << outputFile << std::endl;
        return;
    }

    storage.setHeader(header); // Save tree header
    std::string bitStream = "";
    for (size_t i = 0; i < content.size(); ++i) {
        char c = content[i];
        if (huffmanCodes.find(c) != huffmanCodes.end()) {
            bitStream += huffmanCodes[c]; // Add bits to stream
        }
    }

    int leftoverBits = bitStream.size() % 8;
    int paddingBitsToAdd = 0;

    if (leftoverBits != 0) {
        paddingBitsToAdd = 8 - leftoverBits; // Add padding to make full byte
        bitStream.append(paddingBitsToAdd, '0');
    }

    std::string paddingInfo = "";

    int value = paddingBitsToAdd;
    for (int i = 7; i >= 0; --i) {
        int power = 1;

        for (int j = 0; j < i; ++j) {
            power *= 2;
        }

        if (value >= power) {
            paddingInfo += '1';
            value -= power;
        } else {
            paddingInfo += '0';
        }
    }


    storage.insert(paddingInfo); // Write padding first
    for (size_t i = 0; i < bitStream.size(); ++i) {
        storage.insert(std::string(1, bitStream[i])); // Write bits
    }

    std::cout << "File compressed successfully!" << std::endl;
    deleteTree(root);
    root = nullptr;
    storage.close();
}

// Decompressesion
bool Huffman::decompress(const std::string& inputFile, const std::string& outputFile) {
    Storage storage;
    if (!storage.open(inputFile, "read")) {
        std::cerr << "Failed to open input file: " << inputFile << std::endl;
        return false;
    }

    reconstruct_tree(storage.getHeader()); // Load the tree from file

    std::unordered_map<std::string, char> reverseCodes;
    ReverseCodes(root, "", reverseCodes); // Create code map

    std::ofstream outFile(outputFile, std::ios::binary);
    if (!outFile) {
        std::cerr << "Failed to open output file: " << outputFile << std::endl;
        return false;
    }

    std::string totalBits;
    std::string tempBit;
    while (storage.extract(tempBit)) {
        totalBits += tempBit; // Read all bits
    }

    std::string paddingInfo = totalBits.substr(0, 8); // First 8 bits are padding info
    int numPaddingBits = 0;


    for (int i = 0; i < 8; ++i) {
        if (paddingInfo[i] == '1') {
            int power = 7 - i;
            int value = 1;
            for (int j = 0; j < power; ++j) {
                value *= 2;
            }
            numPaddingBits += value;
        }
    }


    totalBits = totalBits.substr(8); // Remove padding header
    if (numPaddingBits > 0) {
        totalBits = totalBits.substr(0, totalBits.size() - numPaddingBits); // Remove padding bits
    }

    std::string currentCode = "";
    for (size_t i = 0; i < totalBits.size(); ++i) {
        currentCode += totalBits[i];
        if (reverseCodes.find(currentCode) != reverseCodes.end()) {
            outFile.put(reverseCodes[currentCode]); // Write decoded character
            currentCode.clear();
        }
    }

    outFile.close();
    storage.close();

    std::cout << "File decompressed successfully!" << std::endl;
    return true;
}