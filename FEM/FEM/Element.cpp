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

Element::Element()
{

}

Element::Element(int index, std::vector<Node> nodeList)
{
  index_ = index;
  size_ = static_cast<int>(nodeList.size());
  nodeList_.resize(size_);
  nodeCoord_.resize(size_, 2);

  for (int i = 0; i < size_; i++) {
    nodeList_(i) = nodeList[i].getIndex();
    nodeCoord_.row(i) = nodeList[i].getGlobalCoord();
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
