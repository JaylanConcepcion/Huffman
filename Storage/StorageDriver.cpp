#include <iostream>
#include <bitset>
#include "Storage.h"

int main()
{
    // Create a new storage object
    Storage *storage = new Storage();

    // Open a StorageDriverTest.txt for writing
    std::string fileName= "StorageDriverTest.txt";

    if (!storage->open(fileName, "write")) {
        std::cout << "There was an error opening the file." << std::endl;
        exit(0);
    }

    storage->setHeader("This is my header");

    // insert a binary string into the storage object
    std::string binary_string = "0110100001101010101111";
    storage->insert(binary_string);

    // insert an additional string into the storage object
    std::string binary_string2 = "1100111";
    storage->insert(binary_string2);

    // output the strings we pushed in
    std::cout << binary_string << binary_string2 << std::endl;

    // close the storage object for writing
    storage->close();

    // open a file for reading
    if (!storage->open(fileName, "read")) {
        std::cout << "There was an error opening the file." << std::endl;
        exit(0);
    };

    std::string header  = storage->getHeader();

    // prep a string variable to pass by reference
    std::string result ="";

    // extract all binary string 8 bits at a time until there is nothing left to read
    while (storage->extract(result)) {
        std::cout << result;
    }

    return 0;
}