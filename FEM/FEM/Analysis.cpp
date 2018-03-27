/**
 * @file Analysis.cpp
 * Implementation of Analysis class.
 *
 * @author Haohang Huang
 * @date Feburary 26, 2018
 */

#include "Analysis.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

Analysis::Analysis()
{
}

Analysis::Analysis(std::string const & fileName) : mesh(fileName)
{
}

Analysis::~Analysis()
{
}

void Analysis::assembleStiffness()
{
    // Initialize global stiffness  matrix
    globalStiffness.resize(2 * mesh.nodeCount(),2 * mesh.nodeCount());
    // Reserve the space by estimating the maximum number of non-zero elements
    // in the sparse matrix. This should actually be No. of elements * 2n * 2n
    // where n is the size of the element type with the most nodes. Here n = 8.
    int maxNum = mesh.elementCount() * 16 * 16;
    globalStiffness.reserve(maxNum);
    globalStiffness.setZero();

    // Assemble global matrix from local matrix of each element
    Element* curr;
    for (int i = 0; i < mesh.elementCount(); i++) {
        curr = mesh.elementArray()[i];
        int size = curr->getSize();// element type, for Q4 element, size=4; for Q8, size=8, etc
        const MatrixXi & nodeList = curr->getNodeList();// the index of nodes belong to this element, e.g., for element8, it will give you a vector contain (10,11,15,14), use this to locate row & column in globalStiffness matrix
        const MatrixXd & localStiffness = curr->localStiffness();
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

    // Sparse matrix operation notes:
    //     m.setZero() to remove all non-zero coefficients
    //     m.rows() to get number of rows
    //     m.cols() to get number of columns
    //     m.coeffRef(i,j) = k to set value to the element already exists;
    //     m.insert(i,j) = k to set value to the element does not already exist;
    //
    // Efficiently create sparse matrix from triplet list/vector (i,j,value):
    //     typedef Eigen::Triplet<double> T;
    //     std::vector<T> tripletList;
    //     int estimation_of_entries;
    //     tripletList.reserve(estimation_of_entries);
    //     tripletList.push_back(T(i,j,value)); // or emplace_back()
    //     globalStiffness.setFromTriplets(tripletList.begin(), tripletList.end());
    // *Limitation: once initialized, the value cannot be modified
}

void Analysis::applyForce()
{
  nodalForce.resize(globalStiffness.cols());
  nodalForce.setZero();
  for (unsigned i = 0; i < mesh.loadNodeList.size(); i++)
      nodalForce(mesh.loadNodeList[i]) = mesh.loadValue[i] * 2 * M_PI; // *2 M_PI due to the axisymmetric property
}

void Analysis::boundaryCondition()
{
    const std::vector<int> & DOFList = mesh.boundaryNodeList;
    const std::vector<double> & boundaryValue = mesh.boundaryValue;
    for (unsigned i = 0; i < DOFList.size(); i++) {
        // Modify stiffness matrix
        double temp = globalStiffness.coeffRef(DOFList[i], DOFList[i]);
        for (int j = 0; j < globalStiffness.cols(); j++) {
            globalStiffness.coeffRef(DOFList[i], j) = 0;
            globalStiffness.coeffRef(j, DOFList[i]) = 0;
        } // the sparse matrix .row() or .col() is read-only, we can only do element-wise assignment
        globalStiffness.coeffRef(DOFList[i], DOFList[i]) = temp;

        // Modify force vector
        nodalForce -= globalStiffness.col(DOFList[i]) * boundaryValue[i]; // if consider temperature effect : *6.5*-0.000001*30
        nodalForce(DOFList[i]) = temp * boundaryValue[i];
    }
}

void Analysis::computeStrainAndStress()
{
    // Idea:
    // 1. For each element, compute the strain at Gaussian points using nodal
    // displacments by e = Bu.
    // 2. Let the nodal strain be unknowns, from equation we can solve for
    // e(Gaussian) = N e(nodal) --> e(nodal) = N^-1 e(Gaussian)
    // where N^-1 is the pesudo inverse of the N matrix
    // 3. Compute stress from strain by sigma = E e
    // 4. Cumulate strain and stress value at each node, and average at the end
    // In detail of step 2:
    // 9 Gaussian points & 8 nodes, we have e(Gaussian) as a 9-by-4 matrix.
    // N is the stacked shape function [N1 N2 ... N8] at 9 Gaussian points, i.e.
    // a 9-by-8 matrix. And e(nodal) is the stacked strain vector of 8 nodes as
    // a 8-by-4 matrix. Since N is not square, pesudo inverse is used to solve
    // the least square system.

    nodalStrain.resize(mesh.nodeCount(), 4);
    nodalStress.resize(mesh.nodeCount(), 4);

    // Traverse each element
    Element* curr;
    int numNodes; // number of nodes belong to the element
    int numGaussianPt; // number of Gaussian points of the element
    for (int i = 0; i < mesh.elementCount(); i++) {
        curr = mesh.elementArray()[i];
        const VectorXi & nodeList = curr->getNodeList();
        numNodes = curr->getSize();
        const std::vector<Vector2d> & GaussianPt = curr->getShape()->gaussianPoint();
        numGaussianPt = static_cast<int>(GaussianPt.size());

        // Assemble the nodal displacement vector for an element
        VectorXd nodeDisp(2 * numNodes);
        for (int j = 0; j < numNodes; j++) {
            Vector2d disp = mesh.nodeArray()[nodeList(j)]->getDisp();
            nodeDisp(2 * j) = disp(0);
            nodeDisp(2 * j + 1) = disp(1);
        }

        MatrixXd strainAtGaussPt(numGaussianPt, 4); // 4 for axisymmetric problem
        MatrixXd shapeAtGaussPt(numGaussianPt, numNodes); // 9x8 matrix for element Q8

        // Compute strain at gaussian points from e = Bu
        for (int g = 0; g < numGaussianPt; g++) {
            MatrixXd B = curr->BMatrix(GaussianPt[g]);
            VectorXd e = B * nodeDisp;
            strainAtGaussPt.row(g) = e.transpose();
            shapeAtGaussPt.row(g) = curr->getShape()->functionVec(GaussianPt[g]).transpose();
        }

        // Solve/extrapolate for nodal strain value via a least square linear system using pesudo inverse
        MatrixXd pesudo = shapeAtGaussPt.completeOrthogonalDecomposition().pseudoInverse(); // pesudo inverse in "Eigen/QR"
        MatrixXd strainAtNodes = pesudo * strainAtGaussPt; // 8x4 matrix

        // Set calculated strain value to 8 nodes
        const MatrixXd & E = curr->EMatrix();
        for (int n = 0; n < numNodes; n++) {
            VectorXd strain = strainAtNodes.row(n);
            VectorXd stress = E * strain;
            mesh.nodeArray()[nodeList(n)]->setStrainAndStress(strain, stress);
        }

    }

    // Write nodal strain and stress
    for (int i = 0; i < mesh.nodeCount(); i++) {
        nodalStrain.row(i) = mesh.nodeArray()[i]->averageStrain().transpose();
        nodalStress.row(i) = mesh.nodeArray()[i]->averageStress().transpose();
    }

}

void Analysis::printDisp() const
{
    std::cout << "Nodal Displacement: ";
    std::cout << std::endl;
    for (int i = 0; i < mesh.nodeCount(); i++) {
      std::cout << "Node " << mesh.nodeArray()[i]->getIndex() << " : " << mesh.nodeArray()[i]->getDisp().transpose() << std::endl;
    }
    std::cout << std::endl;
}

void Analysis::printForce() const
{
    for (int i = 0; i < mesh.nodeCount(); i++) {
      std::cout << "Node " << mesh.nodeArray()[i]->getIndex() << " force: " << mesh.nodeArray()[i]->getForce().transpose() << std::endl;
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

void Analysis::writeToFile(std::string const & fileName) const
{
    // @TODO your code here
}
