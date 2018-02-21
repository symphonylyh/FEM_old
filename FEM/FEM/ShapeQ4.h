//
//  ShapeQ4.h
//  FEM
//
//  Created by 黄浩航 on 10/02/2018.
//  Copyright © 2018 HHH. All rights reserved.
//

#ifndef ShapeQ4_h
#define ShapeQ4_h

#include "Shape.h"

class ShapeQ4 : public Shape
{
  public:
    ShapeQ4();
    ~ShapeQ4();
    MatrixXd shapeFunction(Vector2d & point) const;
    MatrixXd shapeLocalDeriv(Vector2d & point) const;
  private:
    Vector2d nodeArray_[4];
    int numNodes_;
};

#endif /* ShapeQ4_h */
