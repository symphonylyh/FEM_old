//
//  Mesh.cpp
//  FEM
//
//  Created by 黄浩航 on 05/02/2018.
//  Copyright © 2018 HHH. All rights reserved.
//

#include "Mesh.h"
#include <iostream>
#include <fstream>
#include <string>

bool Mesh::dataCount(std::string const & fileName)
{
    std::ifstream inFile(fileName);
    if(!inFile) {
        std::cout << "Cannot open input file." << std::endl;
        return false;
    }

    int nodeCount = 0;
    int elementCount = 0;

    std::string byLine; // separate lines
    while(!inFile.eof()) { // reach end of file
        std::getline(inFile, byLine);
        std::stringstream oneLine(byLine); // get the stream
        // Parse by file structure
        if (byLine[0] == 'v') {
            nodeCount++;
        } else if (byLine[0] == 'f') {
            elementCount++;
        }
    }
    inFile.close();

    nodeCount_ = nodeCount;
    elementCount_ = elementCount;

    return true;
}

bool Mesh::readFromFile(std::string const & fileName)
{
    std::ifstream inFile(fileName);
    if(!inFile) {
        std::cout << "Cannot open input file." << std::endl;
        return false;
    }

    // Matrix initialization
    MatrixXd nodeCoord(nodeCount_,2);
    MatrixXi elementIndex(elementCount_,4);
    int i = 0;
    int j = 0;
    char c; // token for 'v' and 'v'

    std::string byLine;
    while(!inFile.eof()) {
        std::getline(inFile, byLine);
        std::stringstream oneLine(byLine);
        if (byLine[0] == 'v') {
            oneLine >> c >> nodeCoord(i,0) >> nodeCoord(i,1); // ">>" is by default separated by space
            i++;
        } else if (byLine[0] == 'f') {
            oneLine >> c >> elementIndex(j,0) >> elementIndex(j,1) >> elementIndex(j,2) >> elementIndex(j,3);
            j++;
        }
    }
    inFile.close();

    nodeCoord_ = nodeCoord;
    elementIndex_ = elementIndex;

    std::cout << nodeCoord << std::endl;
    std::cout << elementIndex << std::endl;

    return true;

}
