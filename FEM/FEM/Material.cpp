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
{
    int i = 0;
    switch (properties.size()) {
      case 6 : // Isotropic: Modulus, Poisson's ratio, body force (r,z), thermal coefficient, temperature change
      {   double M = properties[i++];
          double v = properties[i++];

          // Compute the stress-strain constitutive matrix
          E_ << 1 - v, v, v, 0,
                v,   1-v, v, 0,
                v,   v,  1-v, 0,
                0,  0,    0,  (1-2*v)/2;
          E_ = E_ * M / (1+v) /(1-2*v);

          break;
      }

      case 9 : // Cross-anisotropic: Modulus R, Modulus Z, Poisson's ratio R, Poisson's ratio Z, Shear Modulus G, body force (r,z), thermal coefficient, temperature change
      {   double Mr = properties[i++];
          double Mz = properties[i++];
          double vr = properties[i++];
          double vz = properties[i++];
          double G = properties[i++];

          // Helper coefficients
          double n = Mr / Mz;
          double m = G / Mz;
          double A = Mz / (1 + vr) / (1 - vr - 2 * n * vz * vz);

          // Compute the stress-strain constitutive matrix
          E_ << n * (1 - n * vz * vz), n * (vr + n * vz * vz), n * vz * (1 + vr), 0,
                n * (vr + n * vz * vz), n * (1 - n * vz * vz), n * vz * (1 + vr), 0,
                n * vz * (1 + vr), n * vz * (1 + vr), 1 - vr * vr, 0,
                0, 0, 0, m * (1 + vr) * (1 - vr - 2 * n * vz * vz);
          E_ = E_ * A;

          break;
      }
    }

    // Assign body force (unit weight)
    bodyForce_ << properties[i], properties[i+1];
    i += 2;

    // Assign thermal parameters
    double alpha  = properties[i++];
    double deltaT = properties[i++]; /** The temperature change (assume same across the entire element) */
    double strain = alpha * deltaT;
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
