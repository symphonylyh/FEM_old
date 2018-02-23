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
    std::vector<Vector2d> gaussianPoint() const;
    std::vector<double> gaussianWeight() const;
  private:
    Vector2d nodeArray_[8];
    int numNodes_;
    std::vector<Vector2d> gaussianPoint_; // it's hard to return an array by gaussianPoint(), so we use vector. And you can specify size here, do it in initializer list
    int numGaussianPoints_;
    std::vector<double> gaussianWeight_;
};

#endif /* ShapeQ8_h */
