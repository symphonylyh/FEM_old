//
//  ShapeQ8.h
//  FEM
//
//  Created by 黄浩航 on 13/02/2018.
//  Copyright © 2018 HHH. All rights reserved.
//

#ifndef ShapeQ8_h
#define ShapeQ8_h

#include "Shape.h"

class ShapeQ8 : public Shape
{
  public:
    ShapeQ8();
    ~ShapeQ8();
    MatrixXd shapeFunction(Vector2d & point) const;
    MatrixXd shapeLocalDeriv(Vector2d & point) const;
  private:
    Vector2d nodeArray_[8];
    int size_;
};

#endif /* ShapeQ8_h */
