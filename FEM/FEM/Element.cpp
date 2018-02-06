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

int Element::getIndex() 
{
    return index_;
}
