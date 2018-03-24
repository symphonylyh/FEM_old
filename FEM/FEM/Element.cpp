//
//  Element.cpp
//  FEM
//
//  Created by 黄浩航 on 04/02/2018.
//  Copyright © 2018 HHH. All rights reserved.
//

//#include "Eigen/Eigen"
#include "Node.h"
#include "Element.h"

//using namespace Eigen;

Element::Element() : poissonRatio(0.5), modulus(1.0)
{

}

Element::Element(int index, std::vector<int> & nodeList, Node* meshNode) // @TODO previously here I pass in vector<Node> which is very expensive, now I pass in vector<int> & and a pointer the pool of nodes create in mesh
{
  index_ = index;
  size_ = static_cast<int>(nodeList.size());
  nodeList_.resize(size_);
  nodeCoord_.resize(size_, 2);

  for (int i = 0; i < size_; i++) {
    // nodeList_(i) = nodeList[i].getIndex();
    // nodeCoord_.row(i) = nodeList[i].getGlobalCoord();
    nodeList_(i) = nodeList[i];
    nodeCoord_.row(i) = meshNode[nodeList[i]].getGlobalCoord();
  }

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

void Element::clear_()
{

}

void Element::copy_(Element const & other)
{
  index_ = other.index_;
  nodeList_ = other.nodeList_;
}

int Element::getIndex() const
{
    return index_;
}

void Element::setIndex(int index)
{
    index_ = index;
}

int Element::getSize() const
{
    return size_; // static_cast will convert the size (usually 64-bit unsigned int) to 32-bit signed int
}

VectorXi Element::getNodeList() const
{
    return nodeList_;
}

MatrixXd Element::getNodeCoord() const
{
    return nodeCoord_;
}

void Element::computeEMatrix()
{
    E.resize(4,4);
    double M = modulus;
    double v = poissonRatio;
    E << 1 - v, v, v, 0,
          v,   1-v, v, 0,
          v,   v,  1-v, 0,
          0,  0,    0,  (1-2*v)/2;
    E = E * M / (1+v) /(1-2*v);
}

void Element::setPoissonRatio(double v)
{
    poissonRatio = v;
}

void Element::setModulus(double E)
{
    modulus = E;
}
