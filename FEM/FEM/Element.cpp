//
//  Element.cpp
//  FEM
//
//  Created by 黄浩航 on 04/02/2018.
//  Copyright © 2018 HHH. All rights reserved.
//

#include "Eigen/Eigen"
#include "Node.h"
#include "Element.h"

using namespace Eigen;

Element::Element()
{

}

Element::Element(int index, std::vector<Node> nodeList)
{
  index_ = index;
  nodeList_ = nodeList;
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
