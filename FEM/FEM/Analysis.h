//
//  Analysis.h
//  FEM
//
//  Created by 黄浩航 on 26/02/2018.
//  Copyright © 2018 HHH. All rights reserved.
//

#ifndef Analysis_h
#define Analysis_h

#include "Mesh.h"

class Analysis
{
  public:
    Analysis(std::string const & fileName);
    SparseMatrix<double>* assembleStiffness(); // helper function for the ctor
    Mesh& getMesh();
    SparseMatrix<double>& getGlobalStiffness(); // assemble stiffness matrix is public for all methods, no need to be virtual

    virtual ~Analysis();

    //Jiayi
    virtual void modifiedStiffness_y() = 0;
    virtual void modifiedStiffness_x() = 0;
    virtual VectorXd assembleAppliedForce() const = 0;
  private:
    Mesh mesh_;
    SparseMatrix<double>* globalStiffness_;
};

#endif /* Analysis_h */
