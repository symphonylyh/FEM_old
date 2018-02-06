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

using namespace Eigen;

Node::Node(int index, double x, double y)
{
    globalCoord_ << x, y;
    index_ = index;
    fixed_ = false;
}

void Node::fixBoundary()
{
  fixed_ = true;
}

void Node::setLocal(double xi, double eta)
{
  localCoord_ << 0, 0;
}

void Node::setDisp(double u, double v)
{
  if (!fixed_) {
    disp_ << u, v;
  } else {
    std::cout << "This node is fixed at boundary. Assignning displacement failed!" << std::endl;
  }

}

Vector2d Node::getGlobalCoord()
{
    return globalCoord_;
}

Vector2d Node::getLocalCoord()
{
    return localCoord_;
}

Vector2d Node::getDisp()
{
    return disp_;
}
