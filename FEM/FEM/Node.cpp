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

Node::Node() : strain_(VectorXd::Zero(4)), stress_(VectorXd::Zero(4)), averageCount_(0) // @TODO revised here on 3/17
{
}

Node::Node(int index, double x, double y) :  Node()
{
    //globalCoord_.resize(1,2);
    globalCoord_ << x, y;
    //disp_.resize(1,2);
    disp_ << 0, 0;
    index_ = index;
    fixed_ = 0; // Construct a free node with 0 displacement by default
    // strain_ = VectorXd::Zero(4);
    // stress_ = VectorXd::Zero(4);
    // strainAverageCount_ = 0;
    // stressAverageCount_ = 0;
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
    fixed_ = other.fixed_;
    globalCoord_ = other.globalCoord_;
    localCoord_ = other.localCoord_;
    disp_ = other.disp_;
    force_ = other.force_;
}

void Node::setIndex(int index)
{
    index_ = index;
}

void Node::setBoundaryX()
{
    //disp_(0,0) = 0;
    fixed_ = 1;
}

void Node::setBoundaryY()
{
    //disp_(0,1) = 0;
    fixed_ = 2;
}

void Node::setBoundaryAll()
{
    //disp_(0,0) = 0;
    //disp_(0,1) = 0;
    fixed_ = 3;
}

void Node::setGlobalCoord(double x, double y)
{
    //globalCoord_.resize(1,2);
    globalCoord_ << x, y;
}

void Node::setLocalCoord(double xi, double eta)
{
    //localCoord_.resize(1,2);
    localCoord_ << xi, eta;
}

void Node::setDisp(double u, double v)
{
    if (!fixed_) {
      //disp_.resize(1,2);
      disp_ << u, v;
    } else {
      std::cout << "Node " << index_ << " is fixed at status " << fixed_ << ". Assigning displacement failed! (1-X; 2-Y; 3-Both)" << std::endl;
    }
}

void Node::setForce(double Fx, double Fy)
{
    //force_.resize(1,2);
    force_ << Fx, Fy;
}

void Node::setStrainAndStress(VectorXd & strain, VectorXd & stress)
{
    strain_ += strain;
    stress_ += stress;
    averageCount_++;
}

VectorXd Node::averageStrain() {
    strain_ /= averageCount_;
    return strain_;
}

VectorXd Node::averageStress() {
    stress_ /= averageCount_;
    return stress_;
}

int Node::getIndex() const
{
    return index_;
}

Vector2d Node::getGlobalCoord() const
{
    return globalCoord_;
}

Vector2d Node::getLocalCoord() const
{
    return localCoord_;
}

Vector2d Node::getDisp() const
{
    return disp_;
}

Vector2d Node::getForce() const
{
    return force_;
}
