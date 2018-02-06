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

Node::Node()
{

}

Node::Node(int index, double x, double y)
{
    globalCoord_.resize(1,2);
    globalCoord_ << x, y;
    index_ = index;
    fixed_ = false;
}

Node::Node(Node const & other)
{
  copy_(other);
}

Node const & Node::operator=(Node const & other)
{
  if (this != &other) {
    clear_();
    copy_(other);
  }
  return *this;
}

Node::~Node()
{
  clear_();
}

void Node::clear_()
{

}

void Node::copy_(Node const & other)
{
  index_ = other.index_;
  globalCoord_ = other.globalCoord_;
  localCoord_ = other.localCoord_;
  disp_ = other.disp_;
  fixed_ = other.fixed_;
}

void Node::fixBoundary()
{
  fixed_ = true;
}

void Node::setLocal(double xi, double eta)
{
  localCoord_.resize(1,2);
  localCoord_ << 0, 0;
}

void Node::setDisp(double u, double v)
{
  if (!fixed_) {
    disp_.resize(1,2);
    disp_ << u, v;
  } else {
    std::cout << "This node is fixed at boundary. Assignning displacement failed!" << std::endl;
  }

}

MatrixX2d Node::getGlobalCoord() const
{
    return globalCoord_;
}

MatrixX2d Node::getLocalCoord()
{
    return localCoord_;
}

MatrixX2d Node::getDisp()
{
    return disp_;
}
