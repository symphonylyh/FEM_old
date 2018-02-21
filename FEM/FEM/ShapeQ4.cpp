//
//  Shape.cpp
//  FEM
//
//  Created by 黄浩航 on 10/02/2018.
//  Copyright © 2018 HHH. All rights reserved.
//

#include "ShapeQ4.h"

ShapeQ4::ShapeQ4()
{
    // Four corner nodes only
    nodeArray_[0] << -1, -1;
    nodeArray_[1] << 1, -1;
    nodeArray_[2] << 1, 1;
    nodeArray_[3] << -1, 1;
    numNodes_ = 4;
}

ShapeQ4::~ShapeQ4()
{

}

MatrixXd ShapeQ4::shapeFunction(Vector2d & point) const
{
    MatrixXd result = MatrixXd::Zero(2, 2 * numNodes_); // 2-D, 4-Node
    for (int i = 0; i < numNodes_; i++) {
        // Ni = (1+xi_i*xi)(1+eta_i*eta)/4
        double Ni = (1 + nodeArray_[i](0) * point(0)) * (1 + nodeArray_[0](1) * point(1)) / 4;
        result(0, 2 * i) = Ni;
        result(1, 2 * i + 1) = Ni;
    }
    return result;
}

MatrixXd ShapeQ4::shapeLocalDeriv(Vector2d & point) const
{
    MatrixXd result(numNodes_, 2);
    for (int i = 0; i < numNodes_; i++) {
      // d(Ni)/d(xi)=xi_i*(1+eta_i*eta)/4
      result(i, 0) = nodeArray_[i](0) * (1 + nodeArray_[i](1) * point(1)) / 4;
      // d(Ni)/d(eta)=eta_i*(1+xi_i*xi)/4
      result(i, 1) = nodeArray_[i](1) * (1 + nodeArray_[i](0) * point(0)) / 4;
    }
    return result;
}
