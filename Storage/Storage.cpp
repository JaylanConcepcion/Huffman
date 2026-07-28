#include "Storage.h"

Storage::Storage() {
    // set the buffer to empty.
    buffer = "";
}

bool Storage::open(std::string file_name, std::string mode) {
    // Open the file in read or write mode.
    if ("write" == mode ) {
        this->mode = "write";
        file.open(file_name, std::ios::out | std::ios::binary);
    } else if ("read" == mode) {
        this->mode = "read";
        file.open(file_name, std::ios::in | std::ios::binary);
    } else {
        // return false if mode is not set to read or write
        return false;
    }
    // return false if there is a problem opening the file.
    return !file.fail();
}

bool Storage::close() {
    // if the file is in write mode be sure to store the remaining buffer before closing the file.
    if (mode == "write" && buffer.size() > 0) {
        // create a bits variable for the last bit of bits
        std::bitset<8> bits(buffer);
        // shift the bits over to pad the end with 0s
        unsigned long shift = 8 - buffer.size();
        bits <<= shift;
        unsigned long value = bits.to_ulong();
        file.write(reinterpret_cast<const char*>(&value), 1);

    }
    // close the file
    file.close();
    return true;
}

void Storage::setHeader(std::string header) {
    unsigned int size = header.size();
    file.write(reinterpret_cast<const char*>(&size), 4);
    file << header;

}

std::string Storage::getHeader() {
    unsigned int size;
    file.read(reinterpret_cast<char *>(&size), 4);

    char header[size + 1];
    file.read(reinterpret_cast<char *>(&header), size);
    header[size] = '\0';
    std::string result = std::string(header);
    return result;
}

void Storage::insert(std::string binary_string) {
    // add the incoming string to the buffer string
//    std::cout << "--" << binary_string << std::endl;
    buffer = buffer + binary_string;

    // write out bits 8 at a time
    while(buffer.size() >= 8) {
//        std::cout << buffer << std::endl;
        // get the next 8 bits
        std::string bit_string = buffer.substr(0,8);
        // remove the next 8 bits from the buffer
        buffer = buffer.substr(8, buffer.size());
        // save them in a bitset
        std::bitset<8> bits(bit_string);
        // store the value of the bits
        unsigned long value = bits.to_ulong();
        file.write(reinterpret_cast<const char*>(&value), 1);
    }
}


bool Storage::extract(std::string &binary_string) {
    // read the next 8 bits in to a char variable.
    unsigned char p;
    file.read(reinterpret_cast<char *>(&p), 1);
    // if it's the end of the file return false
    if (file.eof()) {
        return false;
    }
    // convert the char to a bitset
    std::bitset<8> bits(p);
    // convert the bitset to a binary string
    // and store it in the binary string variable
    binary_string = bits.to_string();
    // return true for success
    return true;
}




