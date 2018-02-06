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
    Node(int index, double x, double y);  // set index and global coordinates
    void fixBoundary();
    void setLocal(double xi, double eta); // set local coordinates
    void setDisp(double u, double v); // set displacement
    Vector2d getGlobalCoord();
    Vector2d getLocalCoord();
    Vector2d getDisp();

private:
    int index_;
    Vector2d globalCoord_; // Global X-Y Coordinates
    Vector2d localCoord_; // Local xi-eta Coordinates
    Vector2d disp_; // displacement in 2D
    bool fixed_; // fixed boundary: 0-free, 1-fixed


};

#endif /* Node_h */
