/**
 * @file Material.h
 * Material class for the element properties.
 *
 * @author Haohang Huang
 * @date Apr 27, 2018
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
     * @param properties A list of the property parameters.
     */
    Material(const std::vector<double> & properties);

    /**
     * Get the stress-strain constitutive matrix of the element.
     *
     * @return The 4-by-4 E matrix.
     */
    const MatrixXd & EMatrix() const;

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

  protected:
    /** The modulus */
    double modulus_;

    /** The Poisson's ratio */
    double poissonRatio_;

    /** The 4-by-4 stress-strain constitutive matrix sigma = E * e */
    MatrixXd E_;

    /** The body force (unit weight) */
    Vector2d bodyForce_;

    /** The coefficient of thermal effect */
    double thermalCoeff_;

    /** The temperature change (assume same across the entire element) */
    double deltaT_;

    /** The thermal strain */
    VectorXd thermalStrain_;

};

#endif /* Material_h */
