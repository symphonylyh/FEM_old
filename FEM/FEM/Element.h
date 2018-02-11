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
    Element(int index, std::vector<Node> nodeList);

    // Big Three
    Element(Element const & other);
    Element const & operator=(Element const & other);
    virtual ~Element();

    int getIndex() const;
    virtual MatrixXd localStiffness() const = 0;

  private:
    int index_;
    std::vector <Node> nodeList_;

    // Helper functions
    void clear_();
    void copy_(Element const & other);


};
#endif /* Element_h */
