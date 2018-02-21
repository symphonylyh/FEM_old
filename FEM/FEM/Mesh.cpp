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

SparseMatrix<double> Mesh::assembleStiffness() const
{
    // initialize sparse matrix
    SparseMatrix<double> globalStiffness(2 * nodeCount_,2 * nodeCount_);
    globalStiffness.setZero();

    for (int i = 0; i < elementCount_; i++) {
        int size = meshElement_[i]->getSize();// element type, for Q4 element, size=4; for Q8, size=8, etc
        MatrixXi nodeList = meshElement_[i]->printNodeList();// the index of nodes belong to this element, e.g., for element8, it will give you a vector contain (10,11,15,14), use this for your globalStiffness matrix's location
        MatrixXd localStiffness = meshElement_[i]->localStiffness();
        for (int j = 0; j < size; j++){
            for (int k = 0; k < size; k++){
                globalStiffness.coeffRef(2 * nodeList(j), 2 * nodeList(k)) += localStiffness(2 * j , 2 * k);
                globalStiffness.coeffRef(2 * nodeList(j) + 1, 2 * nodeList(k) + 1) += localStiffness(2 * j + 1, 2 * k + 1);
            }
        }
    }
    globalStiffness.makeCompressed();
    return globalStiffness;
    /* Sparse matrix operation notes:
    m.setZero() to remove all non-zero coefficients
    m.rows() to get number of rows
    m.cols() to get number of columns
    m.coeffRef(i,j) = k to set value to the element already exists;
    m.insert(i,j) = k to set value to the element does not already exist;
    */

    /* Efficiently create sparse matrix from triplet list/vector (i,j,value)
     * limitation: the value cannot be modified
    typedef Eigen::Triplet<double> T;
    std::vector<T> tripletList;
    int estimation_of_entries;
    tripletList.reserve(estimation_of_entries);
    tripletList.push_back(T(i,j,value)); // or emplace_back()
    globalStiffness.setFromTriplets(tripletList.begin(), tripletList.end());
    */
}

Mesh::~Mesh()
{
    delete[] meshNode_; meshNode_ = NULL;
    for (int i = 0; i < elementCount_; i++) {
      delete meshElement_[i]; meshElement_[i] = NULL;
    }
    delete[] meshElement_; meshElement_ = NULL;
}
