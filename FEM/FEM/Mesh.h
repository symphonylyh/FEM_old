//
//  Mesh.h
//  FEM
//
//  Created by 黄浩航 on 05/02/2018.
//  Copyright © 2018 HHH. All rights reserved.
//

#ifndef Mesh_h
#define Mesh_h

#include "Node.h"
#include "Element.h"

class Mesh
{
  public:
    // bool readFromFile(string const & fileName);
  private:
    Node* meshNode_;
    Element* meshFace_;
};

#endif /* Mesh_h */
