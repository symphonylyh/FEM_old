/**
 * @file Material.cpp
 * Implementation of the Material class.
 *
 * @author Haohang Huang
 * @date Apr 27, 2018
 */

#include "Material.h"

Material::Material(const std::vector<double> & properties)
  : E_(MatrixXd::Zero(4,4)), thermalStrain_(VectorXd::Zero(4))
{   // Modulus, Poisson's ratio, body force (r,z), thermal coefficient, temperature change
    double M = properties[0];
    double v = properties[1];
    modulus_ = M;
    poissonRatio_ = v;

    // Compute the stress-strain constitutive matrix
    E_ << 1 - v, v, v, 0,
          v,   1-v, v, 0,
          v,   v,  1-v, 0,
          0,  0,    0,  (1-2*v)/2;
    E_ = E_ * M / (1+v) /(1-2*v);

    // Assign body force (unit weight)
    bodyForce_ << properties[2], properties[3];

    // Assign thermal parameters
    thermalCoeff_ = properties[4];
    deltaT_ = properties[5];
    double strain = thermalCoeff_ * deltaT_;
    thermalStrain_ << strain, strain, strain, 0;

}

const MatrixXd & Material::EMatrix() const
{
    return E_;
}

const Vector2d & Material::bodyForce() const
{
    return bodyForce_;
}

const VectorXd & Material::thermalStrain() const
{
    return thermalStrain_;
}
