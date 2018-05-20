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
    for (int i = 0; i < mesh.elementCount(); i++) {

    }
}

void Nonlinear::solveDisp()
{
    assembleStiffnessAndForce();

    SimplicialLDLT <SparseMatrix<double> > solver;
    solver.compute(globalStiffness);
    nodalDisp = solver.solve(nodalForce);

    // Write into node information
    for (int i = 0; i < mesh.nodeCount(); i++) {
        mesh.nodeArray()[i]->setDisp(nodalDisp(2 * i), nodalDisp(2 * i + 1));
    }

}
