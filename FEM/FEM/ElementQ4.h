//
//  ElementQ4.h
//  FEM
//
//  Created by 黄浩航 on 10/02/2018.
//  Copyright © 2018 HHH. All rights reserved.
//

#ifndef ElementQ4_h
#define ElementQ4_h

#include "Element.h"
#include "ShapeQ4.h"

class ElementQ4 : public Element
{
  public:
    ElementQ4();
    ElementQ4(const int & index, const std::vector<int> & nodeList, Node* const meshNode);
    ~ElementQ4();

    MatrixXd localStiffness();
    MatrixXd jacobian() const;

    Shape* getShape() const;
  private:

};

#endif /* ElementQ4_h */
