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

Mesh::Mesh(std::string const & fileName)
{
    readFromFile(fileName);
}

// Helper function for readFromFile()
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
    dataCount(fileName);

    std::ifstream inFile(fileName);
    if(!inFile) {
        std::cout << "Cannot open input file." << std::endl;
        return false;
    }

    // Create node & element array on heap
    meshNode_ = new Node[nodeCount_];
    meshElement_ = new Element*[elementCount_];

    char c; // token for 'v' and 'f'
    double x, y;
    int i1, i2, i3, i4, i5, i6, i7, i8;

    // Read and create all nodes
    std::string byLine; // this record the line we are at
    for (int i = 0; i < nodeCount_; i++) {
        std::getline(inFile, byLine);
        std::stringstream oneLine(byLine);
        oneLine >> c >> x >> y; // ">>" is by default separated by space
        meshNode_[i] = Node(i, 6*x, 6*y); // @TODO 6* here is temporary, just to match Erol's first case!
    }

    // Read and create all elements
    std::vector<Node> nodeList;
    nodeList.reserve(8); // reserve+emplace_back will behave better. reserve can be the highest possible type number of element. e.g., you can also use reserve(100) here
    for (int i = 0; i < elementCount_; i++) {
        std::getline(inFile, byLine);
        std::stringstream oneLine(byLine);
        oneLine >> c >> i1 >> i2 >> i3 >> i4 >> i5 >> i6 >> i7 >> i8; // should revise this line to a while loop when one line reaches end, and count how many nodes we read in, which tells us the element type (since our mesh can contain different types of element)

        nodeList.emplace_back(meshNode_[i1]);
        nodeList.emplace_back(meshNode_[i2]);
        nodeList.emplace_back(meshNode_[i3]);
        nodeList.emplace_back(meshNode_[i4]);
        nodeList.emplace_back(meshNode_[i5]);
        nodeList.emplace_back(meshNode_[i6]);
        nodeList.emplace_back(meshNode_[i7]);
        nodeList.emplace_back(meshNode_[i8]);
        meshElement_[i] = new ElementQ8(i, nodeList);
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

int Mesh::nodeCount() const
{
    return nodeCount_;
}

int Mesh::elementCount() const
{
    return elementCount_;
}

Node* Mesh::nodeArray() const
{
    return meshNode_;
}

Element** Mesh::elementArray() const
{
    return meshElement_;
}

Mesh::~Mesh()
{
    delete[] meshNode_; meshNode_ = NULL;
    for (int i = 0; i < elementCount_; i++) {
      delete meshElement_[i]; meshElement_[i] = NULL;
    }
    delete[] meshElement_; meshElement_ = NULL;
}
