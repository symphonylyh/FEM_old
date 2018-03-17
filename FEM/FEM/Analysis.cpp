//
//  Analysis.cpp
//  FEM
//
//  Created by 黄浩航 on 26/02/2018.
//  Copyright © 2018 HHH. All rights reserved.
//

#include "Analysis.h"
#include <iostream>
#include "ElementQ8.h"
#include "ShapeQ8.h"

Analysis::Analysis(std::string const & fileName) : mesh(fileName)
{
}

Analysis::~Analysis()
{
}

void Analysis::assembleStiffness()
{
    // Initialize sparse matrix
    globalStiffness.resize(2 * mesh.nodeCount(),2 * mesh.nodeCount());
    // globalStiffness.reserve(nnz); // this should be added, nnz is the estimated non-zero elements number
    globalStiffness.setZero();

    // Assemble global matrix from local matrix of each element
    for (int i = 0; i < mesh.elementCount(); i++) {
        Element* curr = mesh.elementArray()[i];
        int size = curr->getSize();// element type, for Q4 element, size=4; for Q8, size=8, etc
        MatrixXi nodeList = curr->getNodeList();// the index of nodes belong to this element, e.g., for element8, it will give you a vector contain (10,11,15,14), use this to locate row & column in globalStiffness matrix
        MatrixXd localStiffness = curr->localStiffness();
        for (int j = 0; j < size; j++){
            for (int k = 0; k < size; k++){
                globalStiffness.coeffRef(2 * nodeList(j), 2 * nodeList(k)) += localStiffness(2 * j , 2 * k);
                globalStiffness.coeffRef(2 * nodeList(j), 2 * nodeList(k) + 1) += localStiffness(2 * j , 2 * k + 1);
                globalStiffness.coeffRef(2 * nodeList(j) + 1, 2 * nodeList(k)) += localStiffness(2 * j + 1, 2 * k);
                globalStiffness.coeffRef(2 * nodeList(j) + 1, 2 * nodeList(k) + 1) += localStiffness(2 * j + 1, 2 * k + 1);
            }
        }
    }
    globalStiffness.makeCompressed();
}
// SparseMatrix<double> Mesh::assembleStiffness() const
// {
//     // initialize sparse matrix
//     SparseMatrix<double> globalStiffness(2 * nodeCount_,2 * nodeCount_);
//     globalStiffness.setZero();
//
//     for (int i = 0; i < elementCount_; i++) {
//         int size = meshElement_[i]->getSize();// element type, for Q4 element, size=4; for Q8, size=8, etc
//         MatrixXi nodeList = meshElement_[i]->getNodeList();// the index of nodes belong to this element, e.g., for element8, it will give you a vector contain (10,11,15,14), use this for your globalStiffness matrix's location
//         MatrixXd localStiffness = meshElement_[i]->localStiffness();
//         for (int j = 0; j < size; j++){
//             for (int k = 0; k < size; k++){
//                 globalStiffness.coeffRef(2 * nodeList(j), 2 * nodeList(k)) += localStiffness(2 * j , 2 * k);
//                 globalStiffness.coeffRef(2 * nodeList(j) + 1, 2 * nodeList(k) + 1) += localStiffness(2 * j + 1, 2 * k + 1);
//             }
//         }
//     }
//     globalStiffness.makeCompressed();
//     return globalStiffness;
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
// }

void Analysis::applyForce()
{
  nodalForce.resize(globalStiffness.cols());
  nodalForce.setZero();
  nodalForce(67) = -30;
  nodalForce(69) = -30;
  nodalForce(71) = -30;
}

/* This function will modify the global stiffness matrix and force vector based on boundary conditions
  @ DOFList the DOF of the nodes that are restricted as boundary
  @ boundaryValue the boundary values. Can be zero or non-zero
*/
void Analysis::boundaryCondition(std::vector<int> DOFList, std::vector<double> boundaryValue)
{
    for (unsigned i = 0; i < DOFList.size(); i++) {
        // Modify stiffness matrix
        double temp = globalStiffness.coeffRef(DOFList[i], DOFList[i]);
        for (int j = 0; j < globalStiffness.cols(); j++) {
          globalStiffness.coeffRef(DOFList[i], j) = 0;
          globalStiffness.coeffRef(j, DOFList[i]) = 0;
        }
        globalStiffness.coeffRef(DOFList[i], DOFList[i]) = temp;

        // Modify force vector
        nodalForce -= globalStiffness.col(DOFList[i]) * boundaryValue[i]; // if consider temperature effect : *6.5*-0.000001*30
        nodalForce(DOFList[i]) = temp * boundaryValue[i];
    }
}

void Analysis::printDisp() const
{
    for (int i = 0; i < mesh.nodeCount(); i++) {
      std::cout << "Node " << mesh.nodeArray()[i].getIndex() << " displacement: " << mesh.nodeArray()[i].getDisp() << std::endl;
    }
    //std::cout << nodalDisp << std::endl;
}

void Analysis::printForce() const
{
    for (int i = 0; i < mesh.nodeCount(); i++) {
      std::cout << "Node " << mesh.nodeArray()[i].getIndex() << " force: " << mesh.nodeArray()[i].getForce() << std::endl;
    }
}

void Analysis::computeStress()
{

}

void Analysis::computeStrain()
{
    int i = 0, j = 0;
    MatrixXd B = mesh.elementArray()[i]->BMatrix((ElementQ8::shape.gaussianPoint())[j]);
    VectorXd nodeDisp(16);
    nodeDisp << 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1;
    VectorXd e = B * nodeDisp;
    std::cout << e << std::endl;
}

void Analysis::printStress() const
{

}

void Analysis::printStrain() const
{

}
