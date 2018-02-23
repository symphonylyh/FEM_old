//
//  Element.h
//  FEM
//
//  Created by 黄浩航 on 04/02/2018.
//  Copyright © 2018 HHH. All rights reserved.
//

#ifndef Element_h
#define Element_h

//#include "Eigen/Eigen"
#include "Node.h"
//#include <vector>

#include "ShapeQ8.h"

//using namespace Eigen;

class Element
{
  public:
    // Constructors
    Element();
    Element(int index, std::vector<Node> nodeList);

    // Big Three
    Element(Element const & other);
    Element const & operator=(Element const & other);
    virtual ~Element();

    int getIndex() const;
    void setIndex(int index);
    int getSize() const;
    VectorXi getNodeList() const;
    MatrixXd getNodeCoord() const;
    virtual MatrixXd localStiffness() const = 0;
    virtual MatrixXd jacobian() const = 0;

    static ShapeQ8 shapeQ8;

  private:
    int index_;
    int size_;
    VectorXi nodeList_;
    MatrixXd nodeCoord_;
    MatrixXd localStiffness_;

    // Helper functions
    void clear_();
    void copy_(Element const & other);


};
#endif /* Element_h */
