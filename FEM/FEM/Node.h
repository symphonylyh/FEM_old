//
//  Node.h
//  FEM
//
//  Created by 黄浩航 on 04/02/2018.
//  Copyright © 2018 HHH. All rights reserved.
//

#ifndef Node_h
#define Node_h

#include "Eigen/Eigen"

using namespace Eigen;

class Node {

public:
    Node(double x, double y, double xi, double eta);  // set global and local coordinates
    void fixBoundary();
    void setDisp(double u, double v); // set displacement
    Vector2d getGlobalCoord();
    Vector2d getLocalCoord();
    Vector2d getDisp();

private:
    double x_; // Global X-Y Coordinates
    double y_;
    double xi_; // Local xi-eta Coordinates
    double eta_;
    double u_; // displacement in 2D
    double v_;
    bool fixed_; // fixed boundary: 0-free, 1-fixed


};

#endif /* Node_h */
