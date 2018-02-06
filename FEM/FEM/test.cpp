#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::ifstream inFile("meshData.txt");

    if(!inFile) {
        std::cout << "Cannot open input file." << std::endl;
        return 1;
    }

    std::string byLine;
    while(inFile) {
        std::getline(inFile, byLine);
        std::cout << byLine << std::endl;
    }
    std::cout << "hello" << std::endl;
    inFile.close();
    return 0;
}
