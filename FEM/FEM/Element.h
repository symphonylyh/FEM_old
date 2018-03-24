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
    Element(int index, std::vector<int> & nodeList, Node* meshNode);

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
    virtual MatrixXd jacobian(Vector2d & point) const = 0;
    virtual MatrixXd BMatrix(Vector2d & gaussianPoint) const = 0;

    void setPoissonRatio(double v);
    void setModulus(double E);

    double poissonRatio;
    double modulus;
    
  private:
    int index_;
    int size_;
    VectorXi nodeList_;
    MatrixXd nodeCoord_;

    // Helper functions
    void clear_();
    void copy_(Element const & other);


};
#endif /* Element_h */
