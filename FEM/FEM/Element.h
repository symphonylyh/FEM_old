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

class Element
{
  public:
    
  private:
    int type_; // Element type: number of nodes
    int index_;
    Node n1_, n2_, n3_, n4_;



};
#endif /* Element_h */
