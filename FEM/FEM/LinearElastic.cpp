//
//  LinearElastic.cpp
//  FEM
//
//  Created by 黄浩航 on 26/02/2018.
//  Copyright © 2018 HHH. All rights reserved.
//

#include "LinearElastic.h"
#include <iostream>
LinearElastic::LinearElastic(std::string const & fileName) : Analysis(fileName)
{
}

LinearElastic::~LinearElastic()
{
}

void LinearElastic::solveDisp()
{
    assembleStiffness(); globalStiffness_ = globalStiffness; // make a copy
    applyForce();
    // The boundary conditon should finally be read from meshdata.txt file, this is just a temporary way
    // std::vector<int> boundaryNodeList{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,22,36,44,58,66,20,34,42,56,64,78};
    // std::vector<double> boundaryValue{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    boundaryCondition(mesh.boundaryNodeList, mesh.boundaryValue);

    // Option1: SimplicialLDLT <SparseMatrix<double> > solver;
    // Option2: ConjugateGradient <SparseMatrix<double> > solver;
    // SimplicialLDLT is direct solver: Recommended for very sparse and not too large problems (e.g., 2D Poisson eq.)
    // ConjugateGradient is iterative solver: Recommended for large symmetric problems (e.g., 3D Poisson eq.)
    SimplicialLDLT <SparseMatrix<double> > solver;
    solver.compute(globalStiffness);
    nodalDisp = solver.solve(nodalForce);
    responseForce_ = globalStiffness_ * nodalDisp;

    // Write into node information
    for (int i = 0; i < mesh.nodeCount(); i++) {
      mesh.nodeArray()[i].setDisp(nodalDisp(2 * i), nodalDisp(2 * i + 1));
      mesh.nodeArray()[i].setForce(responseForce_(2 * i), responseForce_(2 * i + 1));
    }

}
