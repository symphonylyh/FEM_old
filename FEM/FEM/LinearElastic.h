/**
 * @file LinearElastic.h
 * Derived class from Analysis for linear elastic problems.
 *
 * @author Haohang Huang
 * @date Feburary 26, 2018
 * @note No efficiency optimization required on March 26, 2018.
 */

#ifndef LinearElastic_h
#define LinearElastic_h

#include "Analysis.h"

class LinearElastic : public Analysis
{
  public:
    LinearElastic(std::string const & fileName); // ctor cannot be inherited, should explicitly call base class's ctor in derived class's ctor
    ~LinearElastic();
    void solveDisp();

  private:
    // Some helper member variables or functions can be defined here, e.g. when we
    // start writing nonlinear elastic class
    SparseMatrix<double> globalStiffness_; // make a copy of the original K matrix
    VectorXd responseForce_;

};

#endif /* LinearElastic_h */
