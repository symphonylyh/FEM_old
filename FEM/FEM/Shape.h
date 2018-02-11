//
//  Shape.h
//  FEM
//
//  Created by 黄浩航 on 10/02/2018.
//  Copyright © 2018 HHH. All rights reserved.
//

#ifndef Shape_h
#define Shape_h

#include "Eigen/Eigen"

using namespace Eigen;

/* For polymorphism of a combination of different shapes (e.g., Q3, Q4, Q8), we
 * create a parent class with virtual methods and enable different implementation
 * using inheritance
 */
class Shape
{
  public:

    virtual ~Shape() {}

    virtual MatrixXd shapeFunction(Vector2d & point) const = 0;

    virtual MatrixXd shapeLocalDeriv(Vector2d & point) const = 0;

};

#endif /* Shape_h */
