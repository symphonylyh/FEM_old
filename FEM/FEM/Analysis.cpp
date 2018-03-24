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
#define _USE_MATH_DEFINES


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
  for (unsigned i = 0; i < mesh.loadNodeList.size(); i++)
      nodalForce(mesh.loadNodeList[i]) = mesh.loadValue[i] * 2 * M_PI;
  // nodalForce(67) = -30 * 2 * M_PI;
  // nodalForce(69) = -30 * 2 * M_PI;
  // nodalForce(71) = -30 * 2 * M_PI;
}

/* This function will modify the global stiffness matrix and force vector based on boundary conditions
  @ DOFList the DOF of the nodes that are restricted as boundary
  @ boundaryValue the boundary values. Can be zero or non-zero
*/
void Analysis::boundaryCondition(std::vector<int> & DOFList, std::vector<double> & boundaryValue)
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

void Analysis::computeStrainAndStress()
{
    // temporary here
    // double M = 30000; // a temporary default value
    // double v = 0.3;
    // MatrixXd E(4,4);
    // E << 1 - v, v, v, 0,
    //       v,   1-v, v, 0,
    //       v,   v,  1-v, 0,
    //       0,  0,    0,  (1-2*v)/2;
    // E = E * M / (1+v) /(1-2*v);

    nodalStrain.resize(mesh.nodeCount(), 4);
    nodalStress.resize(mesh.nodeCount(), 4);
    for (int i = 0; i < mesh.elementCount(); i++) {
        // Optimization need here, every time we define a new varible in the loop
        VectorXi nodeList = mesh.elementArray()[i]->getNodeList();
        VectorXd nodeDisp(2 * mesh.elementArray()[i]->getSize());
        MatrixXd E = mesh.elementArray()[i]->E;

        // Assemble node disp vector
        for (int j = 0; j < nodeList.size(); j++) {
            VectorXd disp = mesh.nodeArray()[nodeList(j)].getDisp();
            nodeDisp(2 * j) = disp(0);
            nodeDisp(2 * j + 1) = disp(1);
        }

        MatrixXd strainAtGaussPt(ElementQ8::shape.gaussianPoint().size(), 4); // 9x4 matrix for axisymmetric problem
        MatrixXd shapeAtGaussPt(ElementQ8::shape.gaussianPoint().size(), 8); // 9x8 matrix for element Q8
        // Compute strain at gaussian points
        for (int g = 0; g < ElementQ8::shape.gaussianPoint().size(); g++) {
            MatrixXd B = mesh.elementArray()[i]->BMatrix((ElementQ8::shape.gaussianPoint())[g]);
            VectorXd e = B * nodeDisp;
            strainAtGaussPt.row(g) = e.transpose();
            shapeAtGaussPt.row(g) = ElementQ8::shape.functionVec((ElementQ8::shape.gaussianPoint())[g]).transpose();
        }

        // Solve/extrapolate for nodal strain value via a least square linear system using pesudo inverse
        MatrixXd pesudo = shapeAtGaussPt.completeOrthogonalDecomposition().pseudoInverse();
        MatrixXd strainAtNodes = pesudo * strainAtGaussPt; // 8x4 matrix
        // pesudo inverst in "Eigen/QR"

        // Set calculated strain value to 8 nodes
        for (int n = 0; n < mesh.elementArray()[i]->getSize(); n++) {
            VectorXd strain = strainAtNodes.row(n);
            VectorXd stress = E * strain;
            mesh.nodeArray()[nodeList(n)].setStrainAndStress(strain, stress);
        }

    }

    // Write nodal strain and stress
    for (int i = 0; i < mesh.nodeCount(); i++) {
        nodalStrain.row(i) = mesh.nodeArray()[i].averageStrain().transpose();
        nodalStress.row(i) = mesh.nodeArray()[i].averageStress().transpose();
    }

}

void Analysis::printDisp() const
{
    std::cout << "Nodal Displacement: ";
    std::cout << std::endl;
    for (int i = 0; i < mesh.nodeCount(); i++) {
      std::cout << "Node " << mesh.nodeArray()[i].getIndex() << " : " << mesh.nodeArray()[i].getDisp().transpose() << std::endl;
    }
    std::cout << std::endl;
}

void Analysis::printForce() const
{
    for (int i = 0; i < mesh.nodeCount(); i++) {
      std::cout << "Node " << mesh.nodeArray()[i].getIndex() << " force: " << mesh.nodeArray()[i].getForce().transpose() << std::endl;
    }
    std::cout << std::endl;
}

void Analysis::printStrain() const
{
    std::cout << "Averaged nodal strain: ";
    std::cout << std::endl;
    for (int i = 0; i < mesh.nodeCount(); i++) {
        std::cout << "Node " << i << " : " << nodalStrain.row(i) << std::endl;
    }
    std::cout << std::endl;
    //std::cout << nodalStrain << std::endl; // the whole matrix
}

void Analysis::printStress() const
{
    std::cout << "Averaged nodal stress: ";
    std::cout << std::endl;
    for (int i = 0; i < mesh.nodeCount(); i++) {
        std::cout << "Node " << i << " : " << nodalStress.row(i) << std::endl;
    }
    std::cout << std::endl;
    //std::cout << nodalStress << std::endl; // the whole matrix
}
