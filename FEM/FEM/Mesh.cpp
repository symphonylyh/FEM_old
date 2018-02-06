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

    // Create node & element array on heap
    meshNode_ = new Node[nodeCount_];
    meshElement_ = new Element[elementCount_];
    // Matrix initialization
    MatrixXd nodeCoord(nodeCount_,2);
    MatrixXi elementIndex(elementCount_,4);
    int i = 0;
    int j = 0;
    char c; // token for 'v' and 'f'
    double x, y;
    int i1, i2, i3, i4;

    std::string byLine;
    while(!inFile.eof()) {
        std::getline(inFile, byLine);
        std::stringstream oneLine(byLine);
        if (byLine[0] == 'v') {
            oneLine >> c >> x >> y; // ">>" is by default separated by space
            nodeCoord(i,0) = x;
            nodeCoord(i,1) = y;
            Node tempNode(i, x, y);
            meshNode_[i] = tempNode;
            i++;
        } else if (byLine[0] == 'f') {
            oneLine >> c >> i1 >> i2 >> i3 >> i4;
            elementIndex(j,0) = i1;
            elementIndex(j,1) = i2;
            elementIndex(j,2) = i3;
            elementIndex(j,3) = i4;
            Element tempElement(j, i1, i2, i3, i4);
            meshElement_[j] = tempElement;
            j++;
        }
    }
    inFile.close();

    nodeCoord_ = nodeCoord;
    elementIndex_ = elementIndex;

    return true;

}

Node Mesh::getNode(int index)
{
    return meshNode_[index];
}

Element Mesh::getElement(int index)
{
    return meshElement_[index];
}

MatrixXd Mesh::getNodeCoord(int index)
{
    return nodeCoord_.row(index);
}

MatrixXi Mesh::getElementIndex(int index)
{
    return elementIndex_.row(index);
}

Mesh::~Mesh()
{
    delete[] meshNode_; meshNode_ = NULL;
    delete[] meshElement_; meshElement_ = NULL;
}
