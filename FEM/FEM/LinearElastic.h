//
//  LinearElastic.h
//  FEM
//
//  Created by 黄浩航 on 26/02/2018.
//  Copyright © 2018 HHH. All rights reserved.
//

#ifndef LinearElastic_h
#define LinearElastic_h

#include "Analysis.h"

class LinearElastic : public Analysis
{
  public:
    LinearElastic(std::string const & fileName); // ctor cannot be inherited, should explicitly call base class's ctor in derived class's ctor
    ~LinearElastic();

    void solveDisp();
    /* Since all the information such as mesh, global stiffness, force, etc
    have been made public in Analysis class, by inheritance we have all those info
    in this derived class. To use them, just say mesh, globalStiffness is okay
    because we don't define the member variable of the same name.
    In addition, from a memory allocation view, the base class's memory space is allocated
    inside the memory space of this derived class. But these space are just copies
    from the base class, i.e., if we have a second derived class, these space are
    independent and uniquely of their own.
    */
  private:
    // Some helper member variables or functions can be defined here, e.g. when we
    // start writing nonlinear elastic class
    SparseMatrix<double> globalStiffness_; // make a copy of the original K matrix
    VectorXd responseForce_;

};

#endif /* LinearElastic_h */
