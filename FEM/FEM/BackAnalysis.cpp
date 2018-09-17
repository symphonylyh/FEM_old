/**
 * @file BackAnalysis.cpp
 * Implementation of BackAnalysis class.
 *
 * @author Haohang Huang
 * @date September 16, 2018
 */

#include <iostream>
#include <cmath>
#include "BackAnalysis.h"
#include "Linear.h"

BackAnalysis::BackAnalysis(std::string const & fileName) : Analysis(fileName)
{
}

BackAnalysis::~BackAnalysis()
{
}

void BackAnalysis::solve()
{
    // Data of seven FWD sensors
    VectorXi sensorNode(7);
    VectorXd sensorDisp(7);
    sensorNode << 0, 5, 10, 15, 20, 25, 30; // user input
    sensorDisp << 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1; // user input
    double criteria = 0.1; // user input
    double precision = 0.01; // user input

    bool match = false;
    double compare = 0;
    // while (!match) {
    for (int x = 0; x < 5; x++) {
        // Adjust material properties for current iteration
        // What should be the feedback mechanism for iteration?
        // If computed displacement < sensor displacement, meaning the layer modulus are too high-->decrease; otherwise, increase

        // Material information is stored in Mesh class
        for (auto & m : mesh.materialList) {
            m->adjustModulus(compare * precision); // Note: I add this method for Material{.h,.cpp} and LinearElastic{.h,.cpp}. Don't forget.
        }
        // computed < measured, compare = -1-->layer modulus should decrease, so + (-1) * modulus. Consistent

        // Run analysis
        applyForce();
        assembleStiffness();

        SimplicialLDLT <SparseMatrix<double> > solver;
        solver.compute(globalStiffness);
        nodalDisp = solver.solve(nodalForce);

        // Check convergence
        match = true;
        compare = 0;
        for (int i = 0; i < static_cast<int>(sensorNode.size()); i++) {
            double error = nodalDisp(2 * sensorNode(i) + 1) - sensorDisp(i);
            if ( std::abs(error) > criteria ) {
                // FWD only requires displacement value match. So strain and stress computation is unnecessary between iterations.
                match = false;
                break;
            }
            compare += error < 0 ? -1 : 1; // computed < measured, -1; computed > measured, 1
        }
        compare = compare < 0 ? -1 : 1; // 7 sensors, compare can never be tied as 0. If more sensors are at computed < measured, set compare to -1, and vice versa
    } // exit loop when vertical displacement at all sensor nodes are matching

    // Compute strain and stress and accumulate at each node
    computeStrainAndStress();

    // Average strain and stress at each node
    averageStrainAndStress();
}
