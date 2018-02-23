//
//  ElementQ8.h
//  FEM
//
//  Created by 黄浩航 on 13/02/2018.
//  Copyright © 2018 HHH. All rights reserved.
//

#ifndef ElementQ8_h
#define ElementQ8_h

#include "Element.h"
#include "ShapeQ8.h"

class ElementQ8 : public Element
{
  public:
    ElementQ8();
    ElementQ8(int index, std::vector<Node> nodeList);
    ~ElementQ8();

    MatrixXd localStiffness() const;
    MatrixXd jacobian(Vector2d & point) const;

    static ShapeQ8 shape;

  private:
    MatrixXd termsAtGaussianPt(Vector2d & point, MatrixXd & nodeCoord) const;
};

#endif /* ElementQ8_h */
