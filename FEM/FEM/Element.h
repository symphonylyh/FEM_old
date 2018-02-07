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
    Element(int index, int n1, int n2, int n3, int n4);

    // Big Three
    Element(Element const & other);
    Element const & operator=(Element const & other);
    ~Element();

    int getIndex();
  private:
    int index_, n1_, n2_, n3_, n4_;
    std::vector <Node> NodeList;

    // Helper functions
    void clear_();
    void copy_(Element const & other);


};
#endif /* Element_h */
