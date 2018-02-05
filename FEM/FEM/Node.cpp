//
//  Node.cpp
//  FEM
//
//  Created by 黄浩航 on 04/02/2018.
//  Copyright © 2018 HHH. All rights reserved.
//

#include "Node.h"

Node::Node()
{
    u_ = 0;
    v_ = 0;
}

Node::~Node()
{
    
}

void Node::setDisplacement(double u, double v)
{
    u_ = u;
    v_ = v;
}

double Node::getDisplacement()
{
    return u_;
}
