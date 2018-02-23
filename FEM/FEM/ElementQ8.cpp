//
//  ElementQ8.cpp
//  FEM
//
//  Created by 黄浩航 on 13/02/2018.
//  Copyright © 2018 HHH. All rights reserved.
//

#include "ElementQ8.h"
#define _USE_MATH_DEFINES
#include <cmath>

ShapeQ8 ElementQ8::shape; // static class member should be defined outside all functions

ElementQ8::ElementQ8()
{

}

ElementQ8::ElementQ8(int index, std::vector<Node> nodeList)
  : Element(index, nodeList)
{
}

ElementQ8::~ElementQ8()
{

}

MatrixXd ElementQ8::localStiffness() const
{
    MatrixXd result = MatrixXd::Zero(2 * getSize(), 2 * getSize()); // 16x16, local stiffness matrix
    MatrixXd nodeCoord = getNodeCoord(); // 8x2, stores (r,z) of 8 nodes
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result += 2 * M_PI * termsAtGaussianPt((shape.gaussianPoint())[3 * i + j], nodeCoord) * (shape.gaussianWeight())[3 * i] * (shape.gaussianWeight())[j];
        }
    }
    return result;
}

// Helper function
MatrixXd ElementQ8::termsAtGaussianPt(Vector2d & point, MatrixXd & nodeCoord) const
{ // B.t(xi,eta) * E * B(xi,eta) * J.det(xi,eta) * r(xi,eta)
    MatrixXd B = MatrixXd::Zero(4, 2 * getSize()); // 4x16, B matrix
    VectorXd shapeFunction = shape.functionVec(point); // 8x1 vector
    double radius = shapeFunction.transpose() * nodeCoord.col(1); // term Ni/r where r = sum(Ni*ri)

    MatrixXd localDeriv = shape.localDeriv(point); // 2x8, local deriv (dN/xi, dN/eta) at gaussian points
    MatrixXd jacobian = localDeriv * nodeCoord; // 2x2, jacobian
    MatrixXd jacobianInv = jacobian.inverse(); // 2x2, inverse of jacobian
    MatrixXd globalDeriv = jacobianInv * localDeriv; // 2x8, global deriv (dN/r, dN/z) at gaussian points

    for (int i = 0; i < getSize(); i++) {
        B(0, 2 * i) = globalDeriv(0, i); // dNi/r
        B(1, 2 * i) = shapeFunction(i) / radius; // Ni/r
        B(2, 2 * i + 1) = globalDeriv(1, i); // dNi/dz
        B(3, 2 * i) = globalDeriv(1, i); // dNi/dz
        B(3, 2 * i + 1) = globalDeriv(0, i); // dNi/dr
    }

    double E = 1; // a temporary default value
    MatrixXd result = B.transpose() * E * B * jacobian.determinant() * radius;

    return result;
}

MatrixXd ElementQ8::jacobian(Vector2d & point) const
{
    return shape.localDeriv(point) * getNodeCoord();
}
