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
  private:
    int index_;
    std::vector <Node> NodeList;


};
#endif /* Element_h */
