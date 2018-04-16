/**
 * @file Element.cpp
 * Implementation of Element class.
 *
 * @author Haohang Huang
 * @date Feburary 4, 2018
 */

// for use of M_PI
#define _USE_MATH_DEFINES
#include <cmath>
#include "Node.h"
#include "Element.h"

Element::Element()
{
}

Element::Element(const int & index, const std::vector<int> & nodeList, Node** const meshNode) // @TODO previously here I pass in vector<Node> which is very expensive, now I pass in vector<int> & and a pointer the pool of nodes create in mesh
  : index_(index), size_(static_cast<int>(nodeList.size())),
    nodeList_(size_), nodeCoord_(size_, 2),
    E_(MatrixXd::Zero(4,4)),
    localStiffness_(MatrixXd::Zero(2 * size_, 2 * size_)),
    nodalForce_(VectorXd::Zero(2 * size_))
{ // use initializer list
    for (int i = 0; i < size_; i++) {
      nodeList_(i) = nodeList[i];
      nodeCoord_.row(i) = meshNode[nodeList[i]]->getGlobalCoord();
    }
    // All the material, force, boundary information are later assigned by
    // setMaterial(), so here we only initialize the sizes
}

Element::Element(Element const & other)
{
    copy_(other);
}

Element const & Element::operator=(Element const & other)
{
    if (this != &other) {
        clear_();
        copy_(other);
    }
    return *this;
}

Element::~Element()
{
    clear_();
}

void Element::setMaterial(const std::vector<double> & properties)
{
    double M = properties[0];
    double v = properties[1];
    modulus_ = M;
    poissonRatio_ = v;

    // Compute the stress-strain constitutive matrix
    E_ << 1 - v, v, v, 0,
          v,   1-v, v, 0,
          v,   v,  1-v, 0,
          0,  0,    0,  (1-2*v)/2;
    E_ = E_ * M / (1+v) /(1-2*v);

    // Assign body force (unit weight)
    bodyForce_ << properties[2], properties[3];

    // Assign thermal parameters
    thermalCoeff_ = properties[4];
    deltaT_ = properties[5];
    thermalStrain_.resize(4);
    double strain = thermalCoeff_ * deltaT_;
    thermalStrain_ << strain, strain, strain, 0;
}

const MatrixXd & Element::localStiffness() const
{
    _computeStiffnessAndForce(); // bootstrap the computation of stiffness matrix and force vector. After callling this function, the member variables are all computed
    return localStiffness_;
}

const VectorXd & Element::nodalForce() const
{
    return nodalForce_;
}

const MatrixXd & Element::EMatrix() const
{
    return E_;
}

const VectorXd & Element::thermalStrain() const
{
    return thermalStrain_;
}

MatrixXd Element::jacobian(const Vector2d & point) const
{
    return shape()->functionDeriv(point) * nodeCoord_;
}

double Element::radius(const Vector2d & point) const
{
    return shape()->functionVec(point).transpose() * nodeCoord_.col(0);
}

MatrixXd Element::BMatrix(const Vector2d & point) const
{
    MatrixXd B = MatrixXd::Zero(4, 2 * size_);
    MatrixXd globalDeriv = (shape()->functionDeriv(point) * nodeCoord_).inverse() * shape()->functionDeriv(point);

    for (int n = 0; n < size_; n++) {
        B(0, 2 * n) = globalDeriv(0, n);
        VectorXd N = shape()->functionVec(point);
        B(1, 2 * n) = N(n) / radius(point);
        B(2, 2 * n + 1) = globalDeriv(1, n);
        B(3, 2 * n) = globalDeriv(1, n);
        B(3, 2 * n + 1) = globalDeriv(0, n);
    }

    return B;
}

const int & Element::getIndex() const
{
    return index_;
}

const int & Element::getSize() const
{
    return size_;
}

const VectorXi & Element::getNodeList() const
{
    return nodeList_;
}

const MatrixXd & Element::getNodeCoord() const
{
    return nodeCoord_;
}


void Element::_computeStiffnessAndForce()
{
    for (int i = 0; i < shape()->gaussianPt().size(); i++) {
        // Local stiffness matrix
        // sum 2PI * B^T * E * B * |J| * r * W(i) at all Gaussian points
        localStiffness_ += 2 * M_PI * _BMatrix(i).transpose() * EMatrix() * _BMatrix(i) * _jacobianDet(i) * _radius(i) * shape()->gaussianWt(i);

        // Body force
        // sum 2PI * N^T * F * |J| * r * W(i) at all Gaussian points
        nodalForce_ += 2 * M_PI * shape()->functionMat(i).transpose() * bodyForce_ * _jacobianDet(i) * _radius(i) * shape()->gaussianWt(i);

        // Temperature load
        // sum 2PI * B^T * E * e0 * |J| * r * W(i) at all Gaussian points
        nodalForce_ += 2 * M_PI * _BMatrix(i).transpose() * EMatrix() * thermalStrain_ * _jacobianDet(i) * _radius(i) * shape()->gaussianWt(i);
    }
}

MatrixXd Element::_BMatrix(const int & i) const
{
    // Example dimensions are given for ElementQ8 type
    MatrixXd B = MatrixXd::Zero(4, 2 * size_); // 4x16, B matrix
    // [dN1/dr  0      | dN2/dr  0      | ... ]
    // [N1/r    0      | N2/r    0      | ... ]
    // [0       dN1/dz | 0       dN2/dz | ... ]
    // [dN1/dz  dN1/dr | dN2/dz  dN2/dr | ... ]
    // where dN/dr = J^-1 * dN/dxi, dN/dz = J^-1 * dN/deta

    // 2x8 global derivatives [dN/dr; dN/dz] = 2x2 inversed Jacobian [J^-1] * 2x8 local derivatives [dN/dxi; dN/deta]
    // where 2x2 inversed Jacobian = 2x8 local derivatives [dN/dxi; dN/deta] * 8x2 node coordinates [ri zi]
    MatrixXd globalDeriv = (shape()->functionDeriv(i) * nodeCoord_).inverse() * shape()->functionDeriv(i);

    for (int n = 0; n < size_; n++) {
        B(0, 2 * n) = globalDeriv(0, n); // dNi/dr
        const VectorXd & N = shape()->functionVec(i);
        B(1, 2 * n) = N(n) / _radius(i); // Ni/r // (shape()->functionVec(i))(n);
        B(2, 2 * n + 1) = globalDeriv(1, n); // dNi/dz
        B(3, 2 * n) = globalDeriv(1, n); // dNi/dz
        B(3, 2 * n + 1) = globalDeriv(0, n); // dNi/dr
    }

    return B;
}

double Element::_jacobianDet(const int & i) const
{
    return (shape()->functionDeriv(i) * nodeCoord_).determinant();
}

double Element::_radius(const int & i) const
{ // r = sum(Ni*ri)
    return shape()->functionVec(i).transpose() * nodeCoord_.col(0);
    // (Solved) @BUG here!! used to write nodeCoord.col(1), but for the r in (r,z) coordinates, we need .col(0)!
}

void Element::clear_()
{ // No dynamically allocated memory within Element class, so do nothing
}

void Element::copy_(Element const & other)
{
    // meshNode = other.meshNode;
    index_ = other.index_;
    size_ = other.size_;
    nodeList_ = other.nodeList_;
    nodeCoord_ = other.nodeCoord_;
    E_ = other.E_;
    localStiffness_ = other.localStiffness_;
    nodalForce_ = other.nodalForce_;
    modulus_ = other.modulus_;
    poissonRatio_ = other.poissonRatio_;
    bodyForce_ = other.bodyForce_;
    thermalCoeff_ = other.thermalCoeff_;
    deltaT_ = other.deltaT_;
    thermalStrain_ = other.thermalStrain_;

}
