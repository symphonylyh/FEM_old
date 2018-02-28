//
//  LinearElastic.cpp
//  FEM
//
//  Created by 黄浩航 on 26/02/2018.
//  Copyright © 2018 HHH. All rights reserved.
//

#include "LinearElastic.h"

LinearElastic::LinearElastic(std::string const & fileName) : Analysis(fileName), mesh_(getMesh()), globalStiffness_(getGlobalStiffness()) //
{
}

LinearElastic::~LinearElastic()
{

}

// Jiayi
void LinearElastic::modifiedStiffness_y()
{
    for (int ii = 0; ii < 7; ii++){
        for (int jj = 0; jj < globalStiffness_.cols(); jj++){
            globalStiffness_.coeffRef(2 * ii , jj) = 0;
            if(2*ii == jj){
                globalStiffness_.coeffRef(2 * ii , jj) = 1;
            }
        }
    }
}

void LinearElastic::modifiedStiffness_x()
{
    for (int ii = 0; ii < 7; ii++){
        for (int jj = 0; jj < globalStiffness_.cols(); jj++){
            globalStiffness_.coeffRef(2 * ii +1, jj) = 0;
            if(2*ii + 1 == jj){
                globalStiffness_.coeffRef(2 * ii + 1 , jj) = 1;
            }
        }
    }
}


VectorXd LinearElastic::assembleAppliedForce() const
{
    VectorXd appliedforce = VectorXd::Zero(globalStiffness_.cols());
    appliedforce.setZero();
    appliedforce(66) = 1e6;
    return appliedforce;
}
