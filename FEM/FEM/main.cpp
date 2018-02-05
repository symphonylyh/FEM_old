//
//  main.cpp
//  FEM
//
//  Created by 黄浩航 on 04/02/2018.
//  Copyright © 2018 HHH. All rights reserved.
//

#include <iostream>
#include "Element.h"
#include "Node.h"
#include "Eigen/Eigen"

using namespace Eigen;

int main(int argc, const char * argv[]) {
    MatrixXf m1(3,4);
    m1 = MatrixXf::Zero(3,4);
    std::cout << m1(2,2) << std::endl;
    return 0;
}
