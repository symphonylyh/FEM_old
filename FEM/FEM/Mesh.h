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
#include "ElementQ8.h"
#include "ShapeQ8.h"
#include <vector>

class Mesh
{
  public:
    Mesh(std::string const & fileName);
    bool dataCount(std::string const & fileName); // record the No. of nodes and elements in the mesh
    bool readFromFile(std::string const & fileName); // read the mesh info and store in node & element object
    // Different from Node.h, here we return a reference because the object meshNode_ & meshElement_ are on HEAP!
    Node & getNode(int index) const; // get the node object at given 0-based index
    Element* & getElement(int index) const; // get the element object at given 0-based index
    int nodeCount() const;
    int elementCount() const;
    Node* nodeArray() const;
    Element** elementArray() const;
    // MatrixXd queryPoint(double x, double y); // interpolate disp, stress, strain at query point, should call Shape method, to be implemented...
    //SparseMatrix<double> assembleStiffness() const;
    ~Mesh();

  private:
    int nodeCount_;
    int elementCount_;
    Node* meshNode_;
    Element** meshElement_;
    std::vector<int> boundaryDOFList_; // not node list, but degree of freedom list
    std::vector<double> boundaryDOFValue_;
};

#endif /* Mesh_h */
