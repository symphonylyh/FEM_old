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
    bool dataCount(std::string const & fileName); // record the No. of nodes and elements in the mesh
    bool readFromFile(std::string const & fileName); // read the mesh info and store in node & element object
    Node getNode(int index); // get the node object at given 0-based index
    Element getElement(int index); // get the element object at given 0-based index
    MatrixXd getNodeCoord(int index); // get the node coordinates as a row vector at given 0-based index
    MatrixXi getElementIndex(int index); // get the element-node indices as a row vector at given 0-based index
    // MatrixXd queryPoint(double x, double y); // interpolate disp, stress, strain at query point, should call Shape method, to be implemented...
    ~Mesh();
  private:
    int nodeCount_;
    int elementCount_;
    MatrixXd nodeCoord_;
    MatrixXi elementIndex_;
    Node* meshNode_;
    Element* meshElement_;
};

#endif /* Mesh_h */
