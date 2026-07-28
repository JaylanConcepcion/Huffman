
#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <string>
#include <unordered_map>
#include <queue>
#include "Node.h"
#include "Storage/Storage.h"

class Huffman {
private:
    Node* root; // Pointer to the root of the Huffman tree

    /**
     * Deletes the Huffman tree starting from the given node.
     * @param node Pointer to the root node of the tree.
     */
    void deleteTree(Node* node);

    /**
     * Recursively encodes the Huffman tree into a string.
     * @param node Pointer to the current node.
     * @return Encoded string representation of the tree.
     */
    std::string encodeTree(Node* node);

    /**
     * Recursively decodes the Huffman tree from a string representation.
     * @param data Encoded tree data.
     * @param index Current index in the encoded data string.
     * @return Pointer to the root of the decoded tree.
     */
    Node* decodeTree(const std::string& data, size_t& index);

    /**
     * Helper for decompression: Generates a reverse mapping of codes to characters.
     * @param node Pointer to the current node.
     * @param code Current binary code string.
     * @param code_Char Map to store the reverse mapping of codes to characters.
     */
    void ReverseCodes(Node* node, const std::string& code, std::unordered_map<std::string, char>& code_Char);

public:
    Huffman();
    ~Huffman();

    /**
     * Reads file content and returns it as a string.
     * @param filename Path to the input file.
     * @return Content of the file as a string.
     */
    std::string inFiles(const std::string& filename);

    /**
     * Builds a frequency table for characters in a given text.
     * @param text Input text.
     * @param freqTable Map to store character frequencies.
     */
    void frequencyTable(const std::string& text, std::unordered_map<char, int>& freqTable);

    /**
     * Builds the Huffman tree based on the character frequency table.
     * @param freqTable Map of character frequencies.
     */
    void buildTree(const std::unordered_map<char, int>& freqTable);

    /**
     * Recursively generates Huffman codes for the characters in the tree.
     * @param node Pointer to the current node.
     * @param code Current binary code string.
     * @param huffmanCodes Map to store Huffman codes for each character.
     */
    void making_codes(Node* node, const std::string& code, std::unordered_map<char, std::string>& huffmanCodes);

    /**
     * Reconstructs the Huffman tree from a string header.
     * @param header Encoded tree header.
     */
    void reconstruct_tree(const std::string& header);

    /**
     * Compresses a text file and saves the compressed content.
     * @param inputFile Path to the input text file.
     * @param outputFile Path to save the compressed file.
     */
    void compress(const std::string& inputFile, const std::string& outputFile);

    /**
     * Decompresses a file and restores the original text content.
     * @param inputFile Path to the compressed file.
     * @param outputFile Path to save the decompressed text.
     * @return True if successful, false otherwise.
     */
    bool decompress(const std::string& inputFile, const std::string& outputFile);
};

#endif // HUFFMAN_H