#include <string>
#include <fstream>
#include <bitset>
#include <iostream>
#include <ios>

#ifndef STORAGE_H
#define STORAGE_H

/**
 * Storage is used to store a binary string into a binary file and read it back out again.
 * This Class is intended to be used with the Huffman lab to store the results of a Huffman
 * compressed string.
 * For an example of using the Storage class see the StorageDriver.cpp file.
 */
class Storage {
public:
    Storage();
    /**
     * Opens a file for reading or writing.
     * @param file_name Path to the file to be opened
     * @param mode Must be set to "read" or "write"
     * @return True if the file is opened, false if something goes wrong
     */
    bool open(std::string file_name, std::string mode);

    /**
     * Flushes buffer and closes the file
     * @return
     */
    bool close();

    /**
     * stores the give header in the file.
     * Note: setHeader MUST be called before storing any data to the file.
     * @param header string containing the header to be stored.
     */
    void setHeader(std::string header);

    /**
     * reads and returns a header string from a file.
     * @return header string
     * @see setHeader
     */
    std::string getHeader();

    /**
     * Stores a binary string as a binary file.
     * @param binary_string String made up of only ones and zeros, for example "0011001"
     */
    void insert(std::string binary_string);

    /**
     * Returns the next 8 bits of a binary string
     * @param binary_string The binary string is passed back through the pass by reference parameter
     * @return true as long as there is more to read in the file and false when the end of the file is reached
     */
    bool extract(std::string &binary_string);


private:
    std::string buffer;
    std::fstream file;
    std::string mode;



};


#endif //STORAGE_H
