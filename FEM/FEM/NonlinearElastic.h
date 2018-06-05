/**
 * @file NonlinearElastic.h
 * Derived class from Material for nonlinear elastic materials.
 *
 * @author Haohang Huang
 * @date May 19, 2018
 */

#ifndef NonlinearElastic_h
#define NonlinearElastic_h

#include "Material.h"

class NonlinearElastic : public Material
{
  public:
    /** See the documentation of base class Material. */
    NonlinearElastic(const bool & anisotropy, const bool & nonlinearity, const bool & noTension, const std::vector<double> & properties);
    ~NonlinearElastic();

    double stressDependentModulus(const VectorXd & stress) const;
    MatrixXd EMatrix(const double & modulus) const;

  protected:
    /** Three regression coefficients used in Uzan (1985) resilient model */
    Vector3d coeffUzan;

    /** Poisson's ratio. */
    double v;

};

#endif /* NonlinearElastic_h */
