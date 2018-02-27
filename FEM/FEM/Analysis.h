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
    virtual Mesh& getMesh() = 0;
    virtual SparseMatrix<double>& assembleStiffness() = 0;
    virtual ~Analysis();
};

#endif /* Analysis_h */
