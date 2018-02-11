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
#include "ElementQ4.h"
#include "ShapeQ4.h"

class Mesh
{
  public:
    bool dataCount(std::string const & fileName); // record the No. of nodes and elements in the mesh
    bool readFromFile(std::string const & fileName); // read the mesh info and store in node & element object
    // Different from Node.h, here we return a reference because the object meshNode_ & meshElement_ are on HEAP!
    Node & getNode(int index) const; // get the node object at given 0-based index
    Element* & getElement(int index) const; // get the element object at given 0-based index
    // Matrix nodeCoord_ & elementIndex_ again are on stack, so shouldn't return a reference
    MatrixXd getNodeCoord(int index) const; // get the node coordinates as a row vector at given 0-based index
    MatrixXi getElementIndex(int index) const; // get the element-node indices as a row vector at given 0-based index
    // MatrixXd queryPoint(double x, double y); // interpolate disp, stress, strain at query point, should call Shape method, to be implemented...
    MatrixXd assembleStiffness() const;
    ~Mesh();
  private:
    int nodeCount_;
    int elementCount_;
    MatrixXd nodeCoord_;
    MatrixXi elementIndex_;
    Node* meshNode_;
    Element** meshElement_;
};

#endif /* Mesh_h */
