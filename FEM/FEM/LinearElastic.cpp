/**
 * @file LinearElastic.cpp
 * Implementation of LinearElastic class.
 *
 * @author Haohang Huang
 * @date May 19, 2018
 */

#include "LinearElastic.h"

LinearElastic::LinearElastic(const bool & anisotropy, const bool & nonlinearity, const bool & noTension, const std::vector<double> & properties)
  : Material(anisotropy, nonlinearity, noTension)
{
    // For linear elastic problem, just store a constant E matrix is sufficient, so initialize it in constructor
    int i = 0;
    if (!anisotropy) {
        // Isotropic: Modulus, Poisson's ratio, body force (r,z), thermal coefficient, temperature change
        double M = properties[i++];
        double v = properties[i++];

        modulus_ = M;
        // Compute the stress-strain constitutive matrix
        E_ << 1 - v, v, v, 0,
              v,   1-v, v, 0,
              v,   v,  1-v, 0,
              0,  0,    0,  (1-2*v)/2;
        E_ = E_ * M / (1+v) /(1-2*v);
    }
    else {
        // Cross-anisotropic: Modulus R, Modulus Z, Poisson's ratio R, Poisson's ratio Z, Shear Modulus G, body force (r,z), thermal coefficient, temperature change
        double Mr = properties[i++];
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

LinearElastic::~LinearElastic()
{
}
