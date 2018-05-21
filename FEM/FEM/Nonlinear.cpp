/**
 * @file Nonlinear.cpp
 * Implementation of Nonlinear class.
 *
 * @author Haohang Huang
 * @date May 19, 2018
 */

#include "Nonlinear.h"
#include <cmath>
#include <iostream>

Nonlinear::Nonlinear(std::string const & fileName) : Analysis(fileName), damping(0.3)
{
}

Nonlinear::~Nonlinear()
{
}

bool Nonlinear::computeStressAtGaussPt()
{
    bool convergence = true;
    double sumError = 0;
    double sumModulus = 0;

    Element* curr;
    int numNodes; // number of nodes belong to the element
    int numGaussianPt; // number of Gaussian points of the element
    for (int i = 0; i < mesh.elementCount(); i++) {
        curr = mesh.elementArray()[i];
        Material* material = curr->material();
        if (material->nonlinearity) { // compute stress for nonlinear elastic element only, skip all linear elastic ones
            const VectorXi & nodeList = curr->getNodeList();
            numNodes = curr->getSize();
            numGaussianPt = (int)curr->shape()->gaussianPt().size();

            // Assemble the nodal displacement vector for this element
            VectorXd nodeDisp(2 * numNodes);
            for (int j = 0; j < numNodes; j++) {
                nodeDisp(2 * j) = nodalDisp(2 * nodeList(j));
                nodeDisp(2 * j + 1) = nodalDisp(2 * nodeList(j) + 1);
            }

            // Step 1: Compute stress at gaussian points based on cached M & E from last iteration
            // Step 2: Update new modulus based on the stress from step 1 and mix with old modulus via damping ratio
            // Step 3: Cache the modulus to be used in the next iteration
            for (int g = 0; g < numGaussianPt; g++) {
                MatrixXd B = curr->BMatrix(curr->shape()->gaussianPt(g));
                VectorXd strain = B * nodeDisp; // e = B * u
                double modulus_old = (curr->modulusAtGaussPt)(g); // M_(i-1)
                VectorXd stress = material->EMatrix(modulus_old) * (strain - curr->thermalStrain()); // sigma = E_(i-1) * (e - e0), note that the M and E are both from previous iteration

                double modulus_new = material->stressDependentModulus(principalStress(stress)); // M_i
                double modulus = (1 - damping) * modulus_old + damping * modulus_new; // true M_i after applying damping ratio

                (curr->modulusAtGaussPt)(g) = modulus;

                // Convergence criteria
                // Criteria 1: modulus stabilize within 5% at all Gaussian points
                double error = std::abs(modulus - modulus_old);
                if (error / modulus > 0.05)
                    convergence = false;
                // Criteraia 2: Accumulative modulus error within 0.2%
                sumError += error * error;
                sumModulus += modulus * modulus;
                //Debug
                if (i == 37 && g == 1) {
                    std::cout << "cylindrical stress: " << stress.transpose() << std::endl;
                    std::cout << "principal stress: " << principalStress(stress).transpose() << std::endl;
                    std::cout << "Old modulus: " << modulus_old << std::endl;
                    std::cout << "New modulus: " << modulus_new << std::endl;
                    std::cout << "True modulus: " << modulus << std::endl;
                }
            }

        }

    }
    // std::cout << "Sum Error: " << sumError / sumModulus << std::endl;
    // std::cout << "Modulus Element No.37: " << mesh.elementArray()[37]->modulusAtGaussPt(1) << std::endl;
    return (sumError / sumModulus < 0.002 && convergence) ? true : false;

}

VectorXd Nonlinear::principalStress(const VectorXd & stress) const
{
    MatrixXd tensor(3,3);
    tensor << stress(0), 0, stress(3),
              0, stress(1), 0,
              stress(3), 0, stress(2);
    SelfAdjointEigenSolver<MatrixXd> es(tensor, EigenvaluesOnly);
    return es.eigenvalues();
}

void Nonlinear::solve()
{
    //------------------------- Iteration 0 ------------------------------------
    // Bootstrap the nonlinear scheme with the initial guess of Modulus and E matrix
    assembleStiffnessAndForce(); // this function solve the problem with the guess E (Material will return the default constant E if you haven't updated it yet)

    SimplicialLDLT <SparseMatrix<double> > solver;
    solver.compute(globalStiffness);
    nodalDisp = solver.solve(nodalForce);

    // Traverse each element, compute stress at Gaussian points, and update the M & E for the next iteration
    computeStressAtGaussPt();

    //------------------------- Iteration j ------------------------------------
    bool convergence = false;
    //int i = 0;
    //while (!convergence) { // convergence criteria
    for (int i = 0; i < 10; i++) {
        std::cout << "Iteration No." << i << std::endl;
        assembleStiffnessAndForce();
        SimplicialLDLT <SparseMatrix<double> > solver;
        solver.compute(globalStiffness);
        nodalDisp = solver.solve(nodalForce);
        // std::cout << "Displacement: " << nodalDisp(0) << " " << nodalDisp(1) << std::endl;
        convergence = computeStressAtGaussPt();
    }

    // After convergence, compute the final results
    computeStrainAndStress();
    averageStrainAndStress();

}
