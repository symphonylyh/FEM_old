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
    Node(); // default ctor
    Node(int index, double x, double y);  // set index and global coordinates

    // Big Three
    Node(Node const & other);
    Node const & operator=(Node const & other);
    ~Node();

    void fixBoundary();
    void setLocal(double xi, double eta); // set local coordinates
    void setDisp(double u, double v); // set displacement
    // Previous bug:
    // MatrixX2d & getGlobalCoord() const; // MatrixX2d is allocated on stack, so this is error "return a ref/ptr to stack memory", you should return ref/ptr for objects on heap
    MatrixX2d getGlobalCoord() const; // global coordinates won't be changed, but disp could be
    MatrixX2d getLocalCoord();
    MatrixX2d getDisp();

private:
    int index_;
    MatrixX2d globalCoord_; // Global X-Y Coordinates
    MatrixX2d localCoord_; // Local xi-eta Coordinates
    MatrixX2d disp_; // displacement in 2D
    bool fixed_; // fixed boundary: 0-free, 1-fixed

    // Helper functions
    void clear_();
    void copy_(Node const & other);


};

#endif /* Node_h */
