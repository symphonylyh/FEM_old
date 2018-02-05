//
//  Node.cpp
//  FEM
//
//  Created by 黄浩航 on 04/02/2018.
//  Copyright © 2018 HHH. All rights reserved.
//

#include "Eigen/Eigen"
#include "Node.h"

Node::Node(double x, double y, double xi, double eta)
{
    x_ = x;
    y_ = y;
    xi_ = xi;
    eta_ = eta;
    u_ = 0;
    v_ = 0;
}

void Node::setDisp(double u, double v)
{
    u_ = u;
    v_ = v;
}

Vector2d Node::getGlobalCoord()
{
    Vector2d coord(x_, y_);
    return coord;
}

Vector2d Node::getLocalCoord()
{
    Vector2d coord(xi_, eta_);
    return coord;
}

Vector2d Node::getDisp()
{
    Vector2d disp(u_, v_);
    return disp;
}
