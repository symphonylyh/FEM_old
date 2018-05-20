/**
 * @file Nonlinear.cpp
 * Implementation of Nonlinear class.
 *
 * @author Haohang Huang
 * @date May 19, 2018
 */

#include "Nonlinear.h"

Nonlinear::Nonlinear(std::string const & fileName) : Analysis(fileName)
{
}

Nonlinear::~Nonlinear()
{
}

void Nonlinear::computeStressAtGaussPt()
{
    Element* curr;
    int numNodes; // number of nodes belong to the element
    int numGaussianPt; // number of Gaussian points of the element
    for (int i = 0; i < mesh.elementCount(); i++) {
        curr = mesh.elementArray()[i];
        const VectorXi & nodeList = curr->getNodeList();
        numNodes = curr->getSize();
        numGaussianPt = (int)curr->shape()->gaussianPt().size();

        // Assemble the nodal displacement vector for this element
        VectorXd nodeDisp(2 * numNodes);
        for (int j = 0; j < numNodes; j++) {
            Vector2d disp = mesh.nodeArray()[nodeList(j)]->getDisp();
            nodeDisp(2 * j) = disp(0);
            nodeDisp(2 * j + 1) = disp(1);
        }

        VectorXd strainAtGaussPt(4); // 4 for axisymmetric problem
        VectorXd stressAtGaussPt(4);
        MatrixXd E = material_->EMatrix
        MatrixXd strainAtGaussPt(numGaussianPt, 4);
        MatrixXd shapeAtGaussPt(numGaussianPt, numNodes); // 9x8 matrix for element Q8

        // Compute strain at gaussian points from e = Bu
        for (int g = 0; g < numGaussianPt; g++) {
            MatrixXd B = curr->BMatrix(curr->shape()->gaussianPt(g));
            VectorXd e = B * nodeDisp; // e = B * u
            strainAtGaussPt.row(g) = e.transpose();
            shapeAtGaussPt.row(g) = curr->shape()->functionVec(g).transpose();
        }

        // Solve/extrapolate for nodal strain value via a least square linear system using pesudo inverse
        // Previous attempt: solving the system by pesudo inverse, this might have numerical error
        // MatrixXd pesudo = shapeAtGaussPt.completeOrthogonalDecomposition().pseudoInverse(); // pesudo inverse in "Eigen/QR"
        // MatrixXd strainAtNodes = pesudo * strainAtGaussPt; // 8x4 matrix
        // Current solution: use SVD decomposition
        MatrixXd strainAtNodes = shapeAtGaussPt.bdcSvd(ComputeThinU | ComputeThinV).solve(strainAtGaussPt);

        // Notes on LLS system:
        // Several options for solving a linear least squares system:
        // 1. SVD decomposition
        // VectorXd x = A.bdcSvd(ComputeThinU | ComputeThinV).solve(b);
        // 2. QR decomposition
        // VectorXd x = A.colPivHouseholderQr().solve(b);
        // from fast to slow, unstable to stable: householderQr()-->colPivHouseholderQr()-->fullPivHouseholderQr()
        // 3. Normal equations (A^T*A)*x = A^T*b
        // VectorXd x = (A.transpose() * A).ldlt().solve(A.transpose() * b)

        // Set calculated strain value to 8 nodes
        const MatrixXd & E = curr->EMatrix();
        for (int n = 0; n < numNodes; n++) {
            VectorXd strain = strainAtNodes.row(n);
            VectorXd stress = E * (strain - curr->thermalStrain()); // subtract thermal strain, stress = E * (strain - thermal strain)
            mesh.nodeArray()[nodeList(n)]->setStrainAndStress(strain, stress);
        }

    }
}

void Nonlinear::solve()
{
    //------------------------- Iteration 0 ------------------------------------
    // Bootstrap the nonlinear scheme with the initial guess of Modulus and E matrix
    assembleStiffnessAndForce(); // this function solve the problem with the guess E (Material will return the default constant E if you haven't updated it yet)

    SimplicialLDLT <SparseMatrix<double> > solver;
    solver.compute(globalStiffness);
    nodalDisp = solver.solve(nodalForce);

    // Write into node information
    for (int i = 0; i < mesh.nodeCount(); i++) {
        mesh.nodeArray()[i]->setDisp(nodalDisp(2 * i), nodalDisp(2 * i + 1));
    }

    //------------------------- Iteration j ------------------------------------
    // double error = 1;
    // while (error > 0.05) { // convergence criteria
    //
    // }

    // Traverse each element, compute stress at Gaussian points, and update the M & E for the next iteration
    computeStressAtGaussPt();


}
