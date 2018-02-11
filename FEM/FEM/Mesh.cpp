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
#include <vector>

const int NUM_NODE = 4;

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
    meshElement_ = new Element*[elementCount_];
    // Matrix initialization
    nodeCoord_.resize(nodeCount_,2);
    elementIndex_.resize(elementCount_,4);

    char c; // token for 'v' and 'f'
    double x, y;
    int i1, i2, i3, i4;

    std::string byLine;
    for (int i = 0; i < nodeCount_; i++) {
        std::getline(inFile, byLine);
        std::stringstream oneLine(byLine);
        oneLine >> c >> x >> y; // ">>" is by default separated by space
        nodeCoord_(i,0) = x;
        nodeCoord_(i,1) = y;
        meshNode_[i] = Node(i, x, y);
    }

    std::vector<Node> nodeList;
    nodeList.reserve(4); // reserve+emplace_back will behave better
    for (int i = 0; i < elementCount_; i++) {
        std::getline(inFile, byLine);
        std::stringstream oneLine(byLine);
        oneLine >> c >> i1 >> i2 >> i3 >> i4;
        elementIndex_(i,0) = i1;
        elementIndex_(i,1) = i2;
        elementIndex_(i,2) = i3;
        elementIndex_(i,3) = i4;

        nodeList.emplace_back(meshNode_[i1]);
        nodeList.emplace_back(meshNode_[i2]);
        nodeList.emplace_back(meshNode_[i3]);
        nodeList.emplace_back(meshNode_[i4]);
        meshElement_[i] = new ElementQ4(i, nodeList);
        nodeList.clear(); // clear the data storage, but the vector nodeList is still there and can be recycled
    }

    std::vector<Node> ().swap(nodeList); // enforce free the vector memory to prevent memory leak

    inFile.close();

    return true;

}

Node & Mesh::getNode(int index) const
{
    return meshNode_[index];
}

Element* & Mesh::getElement(int index) const
{
    return meshElement_[index];
}

MatrixXd Mesh::getNodeCoord(int index) const
{
    return nodeCoord_.row(index);
}

MatrixXi Mesh::getElementIndex(int index) const
{
    return elementIndex_.row(index);
}

Mesh::~Mesh()
{
    delete[] meshNode_; meshNode_ = NULL;
    for (int i = 0; i < elementCount_; i++) {
      delete meshElement_[i]; meshElement_[i] = NULL;
    }
    delete[] meshElement_; meshElement_ = NULL;
}
