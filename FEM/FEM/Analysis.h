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
    Analysis(std::string const & fileName); // ctor only initialize mesh info
    virtual ~Analysis();

    void assembleStiffness(); // helper function for the ctor
    void applyForce();
    void boundaryCondition(std::vector<int> DOFList, std::vector<double> boundaryValue);
    virtual void solveDisp() = 0;
    void computeStrainAndStress();
    
    void printDisp() const;
    void printForce() const;
    void printStress() const;
    void printStrain() const;

  protected: // change from private to public. "protected" may be a better choice
    Mesh mesh;
    SparseMatrix<double> globalStiffness;
    VectorXd nodalForce;
    VectorXd nodalDisp;
    MatrixXd nodalStrain;
    MatrixXd nodalStress;



};

#endif /* Analysis_h */
