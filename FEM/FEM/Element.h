//
//  Element.h
//  FEM
//
//  Created by 黄浩航 on 04/02/2018.
//  Copyright © 2018 HHH. All rights reserved.
//

#ifndef Element_h
#define Element_h

#include "Eigen/Eigen"
#include "Node.h"
#include <vector>

using namespace Eigen;

class Element
{
  public:
    Element();
    Element(int index, int n1, int n2, int n3, int n4);
    int getIndex();
  private:
    int index_, n1_, n2_, n3_, n4_;
    std::vector <Node> NodeList;


};
#endif /* Element_h */
