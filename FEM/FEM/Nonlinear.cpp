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

Nonlinear::Nonlinear(std::string const & fileName) : Analysis(fileName), damping(0.7) // adjustable damping ratio
{
}

Nonlinear::~Nonlinear()
{
}

void Nonlinear::solve()
{
    // -------------------------------------------------------------------------
    // --------------- Start of Nonlinear Iteration Scheme ---------------------
    // -------------------------------------------------------------------------
    bool nonlinearConvergence = false;
    int i = 0; // for debug print only
    while (!nonlinearConvergence) { // convergence criteria
    //for (int i = 0; i < 10; i++) {
        std::cout << "Nonlinear Iteration No." << i++ << std::endl;
        // Assemble the K and F based on the mesh information. At 1st iteration, the initial guess modulus M0 will be used; later on at iteration i, the stress-dependent modulus updated from (i - 1) iteratiion will be used
        assembleStiffnessAndForce();

        // Solve K U = F
        SimplicialLDLT <SparseMatrix<double> > solver;
        solver.compute(globalStiffness);
        nodalDisp = solver.solve(nodalForce);

        // Traverse each element, compute stress at Gaussian points, and update the modulus for the next (i + 1) iteration (if current iteration is i)
        nonlinearConvergence = nonlinearIteration();
    }
    // After convergence is achieved at the last iteration, the solved displacment
    // is stored in the protected member of Analysis class -- nodalDisp. And
    // globalStiffness & nodalForce are also pre-cached. K, U, F are all knowns
    // and can be used in the following no tension iteration scheme.
    // -------------------------------------------------------------------------
    // -------------------- End of Nonlinear Scheme ----------------------------
    // -------------------------------------------------------------------------

    // -------------------------------------------------------------------------
    // --------------- Start of No Tension Iteration Scheme --------------------
    // -------------------------------------------------------------------------
    bool tensionConvergence = false;
    SimplicialLDLT <SparseMatrix<double> > solver;
    solver.compute(globalStiffness);
    i = 0;
    while (!tensionConvergence) { // convergence criteria
    // for (int i = 0; i < 2; i++) { // for debug print only
        std::cout << "Tension Iteration No." << i++ << std::endl;
        // Solve K U = F
        // Note 1: the above nonlinear iteration scheme is iteratively solving a
        // series of linear elastic cases, where K should be updated every time.
        // But in the no tension iteration scheme, K remains unchanged as the
        // last iteration in the nonlinear process. We only update the F vector.
        // Note 2: in Eigen, the solver.compute() is a pre-conditioning of matrix,
        // and we can just recycle the solver for current use. Therefore, the
        // solver is placed outside the while loop.
        nodalDisp = solver.solve(nodalForce);

        // Traverse each element, compute stress at Gaussian points, and update the modulus for the next (i + 1) iteration (if current iteration is i)
        tensionConvergence = noTensionIteration();
        std::cout << "Check: " << tensionConvergence << std::endl;
    }
    // -------------------------------------------------------------------------
    // ----------------  End of No Tension Iteration Scheme --------------------
    // -------------------------------------------------------------------------

    // After both material nonlinearity and granular no tension scheme converge,
    // compute the nodal strain and stress from the final displacment results
    computeStrainAndStress();
    averageStrainAndStress();

}

bool Nonlinear::nonlinearIteration()
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
                // For Debug Use
                if (i == 37 && g == 1) { // the granular element at centerline
                    // std::cout << "nodelDisp: " << nodeDisp.transpose() << std::endl;
                    // std::cout << "Strain: " << strain.transpose() << std::endl;
                    // std::cout << "E: " << material->EMatrix(modulus_old) << std::endl;
                    // std::cout << "cylindrical stress: " << stress.transpose() << std::endl;
                    // std::cout << "principal stress: " << principalStress(stress).transpose() << std::endl;
                    std::cout << "Old modulus: " << modulus_old << std::endl;
                    std::cout << "New modulus: " << modulus_new << std::endl;
                    std::cout << "True modulus: " << modulus << std::endl;
                }
            }

        }

    }
    std::cout << "Sum Error: " << sumError / sumModulus << std::endl;
    std::cout << "Modulus Element No.37: " << mesh.elementArray()[37]->modulusAtGaussPt(1) << std::endl;
    return (sumError / sumModulus < 0.002 && convergence) ? true : false;

}

bool Nonlinear::noTensionIteration()
{
    bool convergence = true;

    Element* curr;
    int numNodes; // number of nodes belong to the element
    int numGaussianPt; // number of Gaussian points of the element
    for (int i = 0; i < mesh.elementCount(); i++) {
        curr = mesh.elementArray()[i];
        Material* material = curr->material();
        if (material->noTension) { // compute stress for no tension granular elements only, skip all HMA/subgrade ones
            const VectorXi & nodeList = curr->getNodeList();
            numNodes = curr->getSize();
            numGaussianPt = (int)curr->shape()->gaussianPt().size();

            // Assemble the nodal displacement vector for this element
            VectorXd nodeDisp(2 * numNodes);
            for (int j = 0; j < numNodes; j++) {
                nodeDisp(2 * j) = nodalDisp(2 * nodeList(j));
                nodeDisp(2 * j + 1) = nodalDisp(2 * nodeList(j) + 1);
            }

            // Step 1: Compute stress at gaussian points based on the solved nodal displacement from last iteration
            // Step 2: Compute principal stress and rotation angle based on the stress (sigma r, sigma theta, sigma z, tau rz), and filter out the tensile components based on the limiting tensile strength critera 0.1.
            // Step 3: Counteract the global load vector based on the tensionForce and output the boolean convergence.
            MatrixXd tension(4, numGaussianPt);
            for (int g = 0; g < numGaussianPt; g++) {
                MatrixXd B = curr->BMatrix(curr->shape()->gaussianPt(g));
                VectorXd strain = B * nodeDisp; // e = B * u
                double modulus = (curr->modulusAtGaussPt)(g);
                VectorXd stress = material->EMatrix(modulus) * (strain - curr->thermalStrain()); // sigma = E_(i-1) * (e - e0), note that the M and E are both from previous iteration
                // VectorXd principal = principalStress(stress);

                // Compute principal stress and rotation angle
                double sigma1 = (stress(2)+stress(0))/2+std::sqrt((stress(0)-stress(2))*(stress(0)-stress(2))/4+stress(3)*stress(3));
                double sigma2 = stress(1);
                double sigma3 = (stress(2)+stress(0))/2-std::sqrt((stress(0)-stress(2))*(stress(0)-stress(2))/4+stress(3)*stress(3));
                double theta = std::atan2(-2*stress(3), (stress(0) - stress(2))) / 2;

                // In our FEM routine, + is tension, - is compression
                double limit = 0;
                if (/*g == 4 && */sigma1 > limit && sigma2 > limit && sigma3 > limit)
                    convergence = false;
                double t1 = sigma1 > limit ? sigma1 : 0;
                double t2 = sigma2 > limit ? sigma2 : 0;
                double t3 = sigma3 > limit ? sigma3 : 0;
                double sigma_r = (t1 + t3) / 2 + (t1 - t3) * std::cos(2 * theta) / 2;
                double sigma_t = t2;
                double sigma_z = (t1 + t3) / 2 - (t1 - t3) * std::cos(2 * theta) / 2;
                double tau_rz = (t1 - t3) * std::sin(2 * theta) / 2;
                VectorXd tensionStress(4);
                tensionStress << sigma_r, sigma_t, sigma_z, tau_rz;
                tension.col(g) = tensionStress;
            }
            VectorXd tensionForce = curr->computeTensionForce(tension);

            // Assemble the element tension force to global nodal force
            for (int k = 0; k < numNodes; k++) {
                nodalForce(2 * nodeList(k)) += tensionForce(2 * k);
                nodalForce(2 * nodeList(k) + 1) += tensionForce(2 * k + 1);
            }
        }
    }
    return convergence;
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
