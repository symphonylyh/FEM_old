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

Element::Element(int index, int n1, int n2, int n3, int n4)
{
  index_ = index;
  n1_ = n1;
  n2_ = n2;
  n3_ = n3;
  n4_ = n4;
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
  n1_ = other.n1_;
  n2_ = other.n2_;
  n3_ = other.n3_;
  n4_ = other.n4_;
}

int Element::getIndex()
{
    return index_;
}
