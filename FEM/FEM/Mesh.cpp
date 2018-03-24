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

template<typename T>
void parseLine(std::string const & readLine, std::vector<T> & parseLine)
{
    // Idea:
    // use stringstream to parse the string (separate with " " by default) into segments
    // or if we want to specify the delimiter, use string.find()

    std::stringstream ss(readLine);
    T segment;
    parseLine.clear();
    while (ss >> segment)
        parseLine.push_back(segment);

    // Option2: a more general string segmentation approach
    // void parse(const std::string & s, char delimiter, std::vector<std::string> & v) {
    //     // Note:
    //     // 1. std::string.find(delimiter, start) finds the given "delimiter" in a given range ["start", end]
    //     // 2. std::string.substr(start, length) gives the substring begins at "start" and spans "length". "length" can be oversize
    //     // 3. std::string::npos is a special sign for "not found", typically a very large number
    //     std::string::size_type i = 0; // at head
    //     std::string::size_type j = s.find(delimiter, 0); // find the location of first delimiter
    //     while (j != std::string::npos) { // find until the last delimiter
    //         v.push_back(s.substr(i, j - i)); // parse the substring segment before the delimiter
    //         i = ++j; // move start point to one-pass the found delimiter
    //         j = s.find(delimiter, i); // find next delimiter in the string left
    //     }
    //     if(j == std::string::npos) // if finally no delimiter (reach the end), read in the last segment (an oversized length is ok). Here we use s.length() b/c when there's no delimiter we can just read the whole string.
    //         v.push_back(s.substr(i, s.length()));
    // }
    // // Usage
    // std::string str = "Hello World!";
    // std::vector<std::string> v;
    // parse(str,' ',v); // use space as delimiter
    // for(auto & e : v)
    //     std::cout << e << " "; // output is exactly the same
}

bool Mesh::readFromFile(std::string const & fileName)
{
    std::ifstream file(fileName); // open file as read-only stream // same as std::ifstream file; file.open(fileName); // ifstream is read-only type
    std::string readLine; // for each single line

    // Read the first line of the file as mesh summary info including:
    // Total number of nodes
    // Total number of elements
    // Total number of different property combinations these elements should be assigned to
    // Total number of loads in X-direction
    // Total number of loads in Y-direction
    // Total number of displacement constraints in X-direction
    // Total number of displacement constraints in Y-direction
    std::vector<int> meshSummary;
    std::getline(file, readLine);
    parseLine(readLine, meshSummary);
    nodeCount_ = meshSummary[0];
    elementCount_ = meshSummary[1];
    int elementProperties = meshSummary[2];
    int loadX = meshSummary[3];
    int loadY = meshSummary[4];
    int boundaryX = meshSummary[5];
    int boundaryY = meshSummary[6];
    std::vector<int>().swap(meshSummary);

    // Create node & element array on HEAP
    meshNode_ = new Node[nodeCount_];
    meshElement_ = new Element*[elementCount_];

    // Read node coordinates
    std::vector<double> nodeCoord(2); // 2D
    for (int i = 0; i < nodeCount_; i++) {
        std::getline(file, readLine);
        parseLine(readLine, nodeCoord);
        meshNode_[i] = Node(i, nodeCoord[0], nodeCoord[1]);
    }
    std::vector<double>().swap(nodeCoord); // enforce free the vector memory

    // Read element's node list and create the corresponding element type
    std::vector<int> elementNodeList;
    elementNodeList.reserve(8); // at most Q8 element
    for (int i = 0; i < elementCount_; i++) {
        std::getline(file, readLine);
        parseLine(readLine, elementNodeList);
        // Create instances of different types of element
        switch (elementNodeList.size()) {
            case 3 :
                meshElement_[i] = new ElementQ8(i, elementNodeList, meshNode_); // change to Q3 later
                break;
            case 6 :
                meshElement_[i] = new ElementQ8(i, elementNodeList, meshNode_); // change to Q6 later
                break;
            case 8 :
                meshElement_[i] = new ElementQ8(i, elementNodeList, meshNode_);
                break;
        }
    }
    std::vector<int>().swap(elementNodeList);

    // Read element properties (modulus, Poisson's ratio, thermal parameters, etc)
    std::vector<int> elementList;
    std::vector<double> elementProperty;
    elementList.reserve(elementCount_); // at most all elements
    for (int i = 0; i < elementProperties; i++) {
        std::getline(file, readLine);
        parseLine(readLine, elementList);
        std::getline(file, readLine);
        parseLine(readLine, elementProperty);
        for (int j = 0; j < elementList.size(); j++) {
            meshElement_[elementList[j]]->setModulus(elementProperty[0]);
            meshElement_[elementList[j]]->setPoissonRatio(elementProperty[1]);
        }
    }
    std::vector<int>().swap(elementList);
    std::vector<double>().swap(elementPropertyList);

    // Read X-direction load @TODO this is point load only, more complex load pattern TBD
    std::vector<int> nodeLoadXList;
    std::vector<double> loadX;
    std::getline(file, readLine);
    parseLine(readLine, nodeLoadXList);
    std::getline(file, readLine);
    parseLine(readLine, loadX);
    for (int i = 0; i < nodeLoadXList.size(); i++) {
        meshNode_[nodeLoadXList[i]]->setModulus(elementPropertyList[0]);
        meshElement_[elementList[j]]->setPoissonRatio(elementPropertyList[1]);
    }
    std::vector<int>().swap(elementList);
    std::vector<double>().swap(elementPropertyList);
    // Read boundary

    inFile.close();

    // dataCount(fileName);
    //
    // std::ifstream inFile(fileName);
    // if(!inFile) {
    //     std::cout << "Cannot open input file." << std::endl;
    //     return false;
    // }
    //
    // // Create node & element array on heap
    // meshNode_ = new Node[nodeCount_];
    // meshElement_ = new Element*[elementCount_];
    //
    // char c; // token for 'v' and 'f'
    // double x, y;
    // int i1, i2, i3, i4, i5, i6, i7, i8;
    //
    // // Read and create all nodes
    // std::string byLine; // this record the line we are at
    // for (int i = 0; i < nodeCount_; i++) {
    //     std::getline(inFile, byLine);
    //     std::stringstream oneLine(byLine);
    //     oneLine >> c >> x >> y; // ">>" is by default separated by space
    //     meshNode_[i] = Node(i, 6*x, 6*y); // @TODO 6* here is temporary, just to match Erol's first case!
    // }
    //
    // // Read and create all elements
    // std::vector<Node> nodeList;
    // nodeList.reserve(8); // reserve+emplace_back will behave better. reserve can be the highest possible type number of element. e.g., you can also use reserve(100) here
    // for (int i = 0; i < elementCount_; i++) {
    //     std::getline(inFile, byLine);
    //     std::stringstream oneLine(byLine);
    //     oneLine >> c >> i1 >> i2 >> i3 >> i4 >> i5 >> i6 >> i7 >> i8; // should revise this line to a while loop when one line reaches end, and count how many nodes we read in, which tells us the element type (since our mesh can contain different types of element)
    //
    //     nodeList.emplace_back(meshNode_[i1]);
    //     nodeList.emplace_back(meshNode_[i2]);
    //     nodeList.emplace_back(meshNode_[i3]);
    //     nodeList.emplace_back(meshNode_[i4]);
    //     nodeList.emplace_back(meshNode_[i5]);
    //     nodeList.emplace_back(meshNode_[i6]);
    //     nodeList.emplace_back(meshNode_[i7]);
    //     nodeList.emplace_back(meshNode_[i8]);
    //     meshElement_[i] = new ElementQ8(i, nodeList);
    //     nodeList.clear(); // clear the data storage, but the vector nodeList is still there and can be recycled
    // }
    //
    // std::vector<Node> ().swap(nodeList); // enforce free the vector memory to prevent memory leak
    //
    // inFile.close();
    //
    // return true;

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
