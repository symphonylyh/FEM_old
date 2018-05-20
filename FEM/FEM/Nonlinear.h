/**
 * @file Nonlinear.h
 * Derived class from Analysis for nonlinear elastic problems.
 *
 * @author Haohang Huang
 * @date May 19, 2018
 */

#ifndef Nonlinear_h
#define Nonlinear_h

#include "Analysis.h"

/* Derived class for solving nonlinear elastic problems.
 */
class Nonlinear : public Analysis
{
  public:
    /* See the documentation of base class Analysis.
     */
    Nonlinear(std::string const & fileName); // ctor cannot be inherited, should explicitly call base class's ctor in derived class's ctor
    ~Nonlinear();

    /**
     * Compute principal stresses at Gaussian points, and update the modulus and E matrix for next iterations.
     */
    void computeStressAtGaussPt();
    void solve();

};

#endif /* Nonlinear_h */
