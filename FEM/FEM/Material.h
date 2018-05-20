/**
 * @file Material.h
 * Material class for the element properties.
 *
 * @author Haohang Huang
 * @date Apr 27, 2018
 * @note Material cross-anisotropy added on May 16, 2018.
 */

#ifndef Material_h
#define Material_h

#include "Eigen/Eigen"
#include <vector>

using namespace Eigen;

/** Material class for storing the engineering properties of elements. */
class Material
{
  public:

    /**
     * Custom constructor to create an element material.
     *
     * @param isotropy A sign for material isotropy. 0 if isotropic, 1 if cross-anisotropic.
     * @param linearity A sign for material linearity. 0 if linear elastic, 1 if nonlinear elastic.
     * @param properties A list of the material property parameters.
     */
    Material(const bool & anisotropy, const bool & nonlinearity);

    /**
     * Destructor.
     *
     * @note As an abstract class, the destructor must be virtual.
     */
    virtual ~Material();

    /**
     * Get the modulus of the element.
     *
     * @return The modulus value.
     */
    const double & modulus() const;

    /**
     * Get the stress-strain constitutive matrix of the element.
     *
     * @return The 4-by-4 E matrix.
     */
    const MatrixXd & EMatrix() const;

    /**
     * Update the stress-dependent resilient modulus and E matrix of the element. Pure virtual method to be used in nonlinear scheme.
     */
    virtual void stressDependent(const double & bulk, const double & deviator) const;

    /**
     * Get the body force to be used in the load condition.
     *
     * @return The body force as a 2-by-1 vector for 2D axisymmetric problem.
     */
    const Vector2d & bodyForce() const;

    /**
     * Get the thermal strain to be used in the stress computation.
     *
     * @return The thermal strain as a 4-by-1 vector for 2D axisymmetric problem.
     */
    const VectorXd & thermalStrain() const;

    /** A sign for material isotropy. 0 if isotropic, 1 if cross-anisotropic. */
    bool anisotropy;

    /** A sign for material linearity. 0 if linear elastic, 1 if nonlinear elastic. */
    bool nonlinearity;

  protected:

    /** The Young's/Resilient modulus. */
    double modulus_;

    /** The 4-by-4 stress-strain constitutive matrix sigma = E * e */
    MatrixXd E_;

    /** The body force (unit weight) */
    Vector2d bodyForce_;

    /** The thermal strain */
    VectorXd thermalStrain_;

};

#endif /* Material_h */
