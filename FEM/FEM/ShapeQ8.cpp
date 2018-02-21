//
//  ShapeQ8.cpp
//  FEM
//
//  Created by 黄浩航 on 13/02/2018.
//  Copyright © 2018 HHH. All rights reserved.
//

#include "ShapeQ8.h"

ShapeQ8::ShapeQ8()
{
    // Corner nodes
    nodeArray_[0] << -1, -1;
    nodeArray_[1] << 1, -1;
    nodeArray_[2] << 1, 1;
    nodeArray_[3] << -1, 1;
    // Mid-side nodes
    nodeArray_[4] << 0, -1;
    nodeArray_[5] << 1, 0;
    nodeArray_[6] << 0, 1;
    nodeArray_[7] << -1, 0;
    numNodes_ = 8;
}

ShapeQ8::~ShapeQ8()
{

}

MatrixXd ShapeQ8::shapeFunction(Vector2d & point) const
{ // 2x16 matrix
    MatrixXd result = MatrixXd::Zero(2, 2 * numNodes_); // 2-D, 8-Node
    for (int i = 0; i < numNodes_; i++) {
        double Ni = 0;
        if (i < 4) { // 4 corner nodes
          // Ni = (1+xi_i*xi)(1+eta_i*eta)(xi_i*xi+eta_i*eta-1)/4
          Ni = (1 + nodeArray_[i](0) * point(0)) * (1 + nodeArray_[0](1) * point(1)) * (nodeArray_[i](0) * point(0) + nodeArray_[0](1) * point(1) - 1) / 4;
        }
        if (i == 4 || i == 6) { // xi = 0 mid-side nodes
          // Ni = (1-xi^2)(1+eta_i*eta)/2
          Ni = (1 - point(0) * point(0)) * (1 + nodeArray_[i](1) * point(1)) / 2;
        }
        if (i== 5 || i == 7) { // eta = 0 mid-side nodes
          // Ni = (1+xi_i*xi)(1-eta^2)/2
          Ni = (1 + nodeArray_[i](0) * point(0)) * (1 - point(1) * point(1)) / 2;
        }
        result(0, 2 * i) = Ni;
        result(1, 2 * i + 1) = Ni;
    }
    return result;
}

MatrixXd ShapeQ8::shapeLocalDeriv(Vector2d & point) const
{ // 2x8 matrix
    MatrixXd result(2, numNodes_);
    for (int i = 0; i < numNodes_; i++) {
      if (i < 4) { // 4 corner nodes
        // d(Ni)/d(xi)=xi_i*(1+eta_i*eta)*(2*xi_i*xi+eta_i*eta)/4
        result(0, i) = nodeArray_[i](0) * (1 + nodeArray_[i](1) * point(1)) * (2 * nodeArray_[i](0) * point(0) + nodeArray_[i](1) * point(1))/ 4;
        // d(Ni)/d(eta)=eta_i*(1+xi_i*xi)*(2*eta_i*eta+xi_i*xi)/4
        result(1, i) = nodeArray_[i](1) * (1 + nodeArray_[i](0) * point(0)) * (2 * nodeArray_[i](1) * point(1) + nodeArray_[i](0) * point(0))/ 4;
      }
      if (i == 4 || i == 6) { // xi = 0 mid-side nodes
        // d(Ni)/d(xi)= -xi*(1+eta_i*eta)
        result(0, i) = - point(0) * (1 + nodeArray_[i](1) * point(1));
        // d(Ni)/d(eta)= (1-xi^2)*eta_i/2
        result(1, i) = (1 - point(0) * point(0)) * nodeArray_[i](1) / 2;
      }
      if (i== 5 || i == 7) { // eta = 0 mid-side nodes
        // d(Ni)/d(xi)= (1-eta^2)*xi_i/2
        result(0, i) = (1 - point(1) * point(1)) * nodeArray_[i](0) / 2;
        // d(Ni)/d(eta)= -eta*(1+xi_i*xi)
        result(1, i) = - point(1) * (1 + nodeArray_[i](0) * point(0));
      }
    }
    return result;
}
