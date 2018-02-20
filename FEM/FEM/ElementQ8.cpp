//
//  ElementQ8.cpp
//  FEM
//
//  Created by 黄浩航 on 13/02/2018.
//  Copyright © 2018 HHH. All rights reserved.
//

#include "ElementQ8.h"

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
    
    return MatrixXd::Identity(8,8);
}
