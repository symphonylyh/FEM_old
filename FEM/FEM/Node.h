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
    // Constructors
    Node();
    Node(int index, double x, double y);  // set index and global coordinates

    // Big Three
    Node(Node const & other);
    Node const & operator=(Node const & other);
    ~Node();

    // Set operations
    void setIndex(int index);
    void setBoundaryX();
    void setBoundaryY();
    void setBoundaryAll();
    void setGlobalCoord(double x, double y);
    void setLocalCoord(double xi, double eta);
    void setDisp(double u, double v);
    void setForce(double Fx, double Fy);

    void setStrainAndStress(VectorXd & strain, VectorXd & stress);
    void setStress(VectorXd & stress); // strain and stress averaging can later be combined into one

    VectorXd averageStrain();
    VectorXd averageStress();

    // Get operations
    int getIndex() const;
    // Previous bug:
    // MatrixX2d & getGlobalCoord() const; // MatrixX2d is allocated on stack, so this is error "return a ref/ptr to stack memory", you should return ref/ptr for objects on heap
    // MatrixX2d getGlobalCoord() const;
    // MatrixX2d getLocalCoord() const;
    // MatrixX2d getDisp() const;
    // MatrixX2d getForce() const;
    Vector2d getGlobalCoord() const;
    Vector2d getLocalCoord() const;
    Vector2d getDisp() const;
    Vector2d getForce() const;

private:
    int index_; // 0-based index
    int fixed_; // fixed boundary: 0-free, 1-fixX, 2-fixY, 3-fixAll
    Vector2d globalCoord_; // Global X-Y Coordinates
    Vector2d localCoord_; // Local xi-eta Coordinates
    Vector2d disp_; // displacement in 2D
    Vector2d force_; // force in 2D

    VectorXd strain_;
    VectorXd stress_;
    int averageCount_;

    // Helper functions
    void clear_();
    void copy_(Node const & other);

};

#endif /* Node_h */
