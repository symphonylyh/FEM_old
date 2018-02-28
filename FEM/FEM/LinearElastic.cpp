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
    // for (int ii = 0; ii < 7; ii++){
    //     for (int jj = 0; jj < globalStiffness_.cols(); jj++){
    //         globalStiffness_.coeffRef(2 * ii , jj) = 0;
    //         if(2*ii == jj){
    //             globalStiffness_.coeffRef(2 * ii , jj) = 1;
    //         }
    //     }
    // }

}

void LinearElastic::modifiedStiffness_x()
{
    std::vector<int> bottom{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,22,36,44,58,66,20,34,42,56,64,78};
    for (int i = 0; i < bottom.size(); i++) {
      double temp = globalStiffness_.coeffRef(bottom[i], bottom[i]);
      for (int j = 0; j < globalStiffness_.cols(); j++) {
        globalStiffness_.coeffRef(bottom[i],j) = 0;
      }
      for (int j = 0; j < globalStiffness_.cols(); j++) {
        globalStiffness_.coeffRef(j,bottom[i]) = 0;
      }
      globalStiffness_.coeffRef(bottom[i], bottom[i]) = temp;
    }

    // for (int ii = 0; ii < 7; ii++){
    //     for (int jj = 0; jj < globalStiffness_.cols(); jj++){
    //         globalStiffness_.coeffRef(2 * ii +1, jj) = 0;
    //         if(2*ii + 1 == jj){
    //             globalStiffness_.coeffRef(2 * ii + 1 , jj) = 1;
    //         }
    //     }
    // }
    // std::vector<int> list{7,11,18,22,29,33,10,17,21,28,32,39};
    // for (int ii = 0; ii < list.size(); ii++){
    //     for (int jj = 0; jj < globalStiffness_.cols(); jj++){
    //         globalStiffness_.coeffRef(2 * list[ii] +1, jj) = 0;
    //         if(2*list[ii] + 1 == jj){
    //             globalStiffness_.coeffRef(2 * list[ii] + 1 , jj) = 1;
    //         }
    //     }
    // }
}


VectorXd LinearElastic::assembleAppliedForce() const
{
    VectorXd appliedforce = VectorXd::Zero(globalStiffness_.cols());
    appliedforce.setZero();
    appliedforce(67) = -30;
    appliedforce(69) = -30;
    appliedforce(71) = -30;
    return appliedforce;
}
