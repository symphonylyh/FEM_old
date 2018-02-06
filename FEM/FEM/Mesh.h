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
    bool dataCount(std::string const & fileName);
    bool readFromFile(std::string const & fileName);
  private:
    int nodeCount_;
    int elementCount_;
    MatrixXd nodeCoord_;
    MatrixXi elementIndex_;
    Node* meshNode_;
    Element* meshFace_;
};

#endif /* Mesh_h */
