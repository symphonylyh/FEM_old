//
//  Node.cpp
//  FEM
//
//  Created by 黄浩航 on 04/02/2018.
//  Copyright © 2018 HHH. All rights reserved.
//

#include "Eigen/Eigen"
#include "Node.h"
#include <iostream>

Node::Node(double x, double y, double xi, double eta)
{
    x_ = x;
    y_ = y;
    xi_ = xi;
    eta_ = eta;
    u_ = 0;
    v_ = 0;
    fixed_ = false;
}

void Node::fixBoundary()
{
  fixed_ = true;
}

void Node::setDisp(double u, double v)
{
  if (!fixed_) {
    u_ = u;
    v_ = v;
  } else {
    std::cout << "This node is fixed at boundary. Assignning displacement failed!" << std::endl;
  }

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
