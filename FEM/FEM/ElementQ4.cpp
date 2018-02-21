//
//  ElementQ4.cpp
//  FEM
//
//  Created by 黄浩航 on 10/02/2018.
//  Copyright © 2018 HHH. All rights reserved.
//

#include "ElementQ4.h"

ElementQ4::ElementQ4()
{

}

ElementQ4::ElementQ4(int index, std::vector<Node> nodeList)
  : Element(index, nodeList)
{

}

ElementQ4::~ElementQ4()
{

}

MatrixXd ElementQ4::localStiffness() const
{
   return MatrixXd::Identity(4,4);
}

MatrixXd ElementQ4::jacobian() const
{

    return MatrixXd::Identity(8,8);
}
