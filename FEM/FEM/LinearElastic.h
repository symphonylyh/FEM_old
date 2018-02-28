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
    // LinearElastic(std::string const & fileName);
    // Mesh& getMesh(); // we need to add nodal information to the mesh so this can't be const function
    // SparseMatrix<double>& assembleStiffness();
    ~LinearElastic();
    LinearElastic(std::string const & fileName);
    //Jiayi
    void modifiedStiffness_y();
    void modifiedStiffness_x();
    VectorXd assembleAppliedForce() const;

  private:
    Mesh& mesh_;
    SparseMatrix<double>& globalStiffness_;
};

#endif /* LinearElastic_h */
