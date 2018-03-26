/**
 * @file ElementQ8.cpp
 * Implementation of ElementQ8 class.
 *
 * @author Haohang Huang
 * @date Feburary 13, 2018
 */

#include "ElementQ8.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

// ShapeQ8 ElementQ8::shape; // static class member should be defined outside all functions
ElementQ8::staticMembers ElementQ8::statics;

ElementQ8::ElementQ8()
{

}

ElementQ8::ElementQ8(const int & index, const std::vector<int> & nodeList, Node const * & meshNode)
  : Element(index, nodeList, meshNode)
{
}

ElementQ8::~ElementQ8()
{

}

MatrixXd ElementQ8::localStiffness()
{
    MatrixXd result = MatrixXd::Zero(2 * getSize(), 2 * getSize()); // 16x16, local stiffness matrix
    MatrixXd nodeCoord = getNodeCoord(); // 8x2, stores (r,z) of 8 nodes
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result += 2 * M_PI * termsAtGaussianPt((statics.shape->gaussianPoint())[3 * i + j], nodeCoord) * (statics.shape->gaussianWeight())[i] * (statics.shape->gaussianWeight())[j];
        }
    }
    return result;
}

MatrixXd ElementQ8::BMatrix(const Vector2d & point) const {
  MatrixXd nodeCoord = getNodeCoord();
  MatrixXd B = MatrixXd::Zero(4, 2 * getSize()); // 4x16, B matrix
  VectorXd shapeFunction = statics.shape->functionVec(point); // 8x1 vector
  double radius = shapeFunction.transpose() * nodeCoord.col(0); // term Ni/r where r = sum(Ni*ri)
  // (Solved) @BUG here!! used to write nodeCoord.col(1), but for the r in (r,z) coordinates, we need .col(0)!
  MatrixXd localDeriv = statics.shape->localDeriv(point); // 2x8, local deriv (dN/xi, dN/eta) at gaussian points
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

  return B;
}

Shape* ElementQ8::getShape() const
{
    return statics.shape;
}

// Helper function
MatrixXd ElementQ8::termsAtGaussianPt(const Vector2d & point, MatrixXd & nodeCoord)
{ // B.t(xi,eta) * E * B(xi,eta) * J.det(xi,eta) * r(xi,eta)
    MatrixXd B = MatrixXd::Zero(4, 2 * getSize()); // 4x16, B matrix
    VectorXd shapeFunction = statics.shape->functionVec(point); // 8x1 vector
    double radius = shapeFunction.transpose() * nodeCoord.col(0); // term Ni/r where r = sum(Ni*ri)
    // (Solved) @BUG here!! used to write nodeCoord.col(1), but for the r in (r,z) coordinates, we need .col(0)!
    MatrixXd localDeriv = statics.shape->localDeriv(point); // 2x8, local deriv (dN/xi, dN/eta) at gaussian points
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

    computeEMatrix();
    // double M = modulus; // a temporary default value
    // double v = poissonRatio;
    // MatrixXd E(4,4);
    // E << 1 - v, v, v, 0,
    //       v,   1-v, v, 0,
    //       v,   v,  1-v, 0,
    //       0,  0,    0,  (1-2*v)/2;
    // E = E * M / (1+v) /(1-2*v);
    // EMatrix = E;
    MatrixXd result = B.transpose() * E * B * jacobian.determinant() * radius;

    return result;
}

MatrixXd ElementQ8::jacobian(const Vector2d & point) const
{
    return statics.shape->localDeriv(point) * getNodeCoord();
}
