/**
 * @file NonlinearElastic.h
 * Derived class from Material for nonlinear elastic materials.
 *
 * @author Haohang Huang
 * @date May 19, 2018
 * @note Change input format and Add more resilient model options on October 13, 2018.
 */

#ifndef NonlinearElastic_h
#define NonlinearElastic_h

#include "Material.h"

class NonlinearElastic : public Material
{
  public:
    /** See the documentation of base class Material. */
    NonlinearElastic(const bool & anisotropy, const bool & nonlinearity, const bool & noTension, const std::vector<double> & properties, const int & model, const std::vector<double> & parameters);
    ~NonlinearElastic();

    double stressDependentModulus(const VectorXd & stress) const;
    MatrixXd EMatrix(const double & modulus) const;

  protected:
    int modelNo; /* Designator for resilient model used */
    std::vector<double> coeff; /** Regression coefficients used in the resilient model */

    double v; /** Poisson's ratio for isotropic */

    double Mr; /** Horizontal Modulus for anisotropic */
    double Mz; /** Vertical Modulus for anisotropic */
    double vr; /** Horizontal Poisson's ratio for anisotropic */
    double vz; /** Vertical Poisson's ratio for anisotropic */
    double G; /** Shear Modulus for anisotropic */

    // Since a simplification is that: in nonlinear+anisotropic scheme, we only iterate the vertical modulus, so we'd better record the ratio
    double r_Mr;
    double r_G;
};

#endif /* NonlinearElastic_h */
