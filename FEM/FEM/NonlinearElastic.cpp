/**
 * @file NonlinearElastic.cpp
 * Implementation of NonlinearElastic class.
 *
 * @author Haohang Huang
 * @date May 19, 2018
 */

#include "NonlinearElastic.h"
#include <cmath>
#include <iostream>
NonlinearElastic::NonlinearElastic(const bool & anisotropy, const bool & nonlinearity, const bool & noTension, const std::vector<double> & properties, const int & model, const std::vector<double> & parameters)
  : Material(anisotropy, nonlinearity, noTension), modelNo(model), coeff(parameters)
{
    // Just copy from LinearElastic constructor
    int i = 0;
    if (!anisotropy) { // Isotropic: Modulus, Poisson's ratio, body force (r,z), thermal coefficient, temperature change
        modulus_ = properties[i++];
        // Previous:
        // double v = properties[i++];
        // Now when used in back analysis scheme, poisson's ratio becomes a protected member variable to be cached.
        v = properties[i++];
        vr = 0; vz = 0; Mr = 0; Mz = 0; G = 0; r_Mr = 0; r_G = 0; // silence warning

        // Compute the stress-strain constitutive matrix
        E_ << 1 - v, v, v, 0,
              v,   1-v, v, 0,
              v,   v,  1-v, 0,
              0,  0,    0,  (1-2*v)/2;
        E_ = E_ * modulus_ / (1+v) /(1-2*v);
    }
    else { // Cross-anisotropic: Modulus R, Modulus Z, Poisson's ratio R, Poisson's ratio Z, Shear Modulus G, body force (r,z), thermal coefficient, temperature change
        modulus_ = 0; v = 0; // silence warning
        Mr = properties[i++];
        Mz = properties[i++];
        vr = properties[i++];
        vz = properties[i++];
        G = properties[i++];
        r_Mr = Mr / Mz;
        r_G = G / Mz;

        // Helper coefficients for filling E matrix
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

NonlinearElastic::~NonlinearElastic()
{
}

double NonlinearElastic::stressDependentModulus(const VectorXd & stress) const
{
    // stress(0)-sigma3; stress(1)-sigma2; stress(2)-sigma1
    // Bulk stress: theta = sigma1 + sigma2 + sigma3
    // Deviatoric stress: sigma_d = sigma1 - sigma3
    // Octahedral shear stress (usually meaningful in 3D): tau_oct = sqrt((1-2)^2 + (2-3)^2 + (1-3)^2) / 3, for 2D := sqrt(2)/3 * sigma_d
    double bulk = std::abs( stress(2) + stress(1) + stress(0) );
    double deviator = std::abs( stress(2) - stress(0) );
    double octahedral = std::sqrt(std::pow(stress(2) - stress(1), 2) + std::pow(stress(1) - stress(0), 2) + std::pow(stress(2) - stress(0), 2)) / 3;
    double atm = 14.7; // atmospheric pressure, 101.325 kPa or 14.7 psi

    // Calculated new stress-dependent resilient modulus from different models
    double M = 0;
    switch (modelNo) {
        case 1 :
            // Hicks and Monismith (1971) K-theta model
            // M = k1 * theta^k2
            M = coeff[0] * std::pow(bulk, coeff[1]);
            break;
        case 2 :
            // Uzan (1985) model:
            // M = k1 * theta^k2 * sigma_d^k3
            M = coeff[0] * std::pow(bulk, coeff[1]) * std::pow(deviator, coeff[2]);
            break;
        case 3 :
            // Pezo (1993) UT-Austin model
            // M = k1 * sigma_d^k2 * sigma_3^k3
            M = coeff[0] * std::pow(deviator, coeff[1]) * std::pow(std::abs(stress(0)), coeff[2]);
            break;
        case 4 :
            // NCHRP 1-37A (2004) MEPDG model
            // M = k1 * p_a * (theta/p_a)^k2 * (tau_oct/p_a + 1)^k3
            M = coeff[0] * atm * std::pow(bulk/atm, coeff[1]) * std::pow(octahedral/atm + 1, coeff[2]);
            break;
    }
    return M;
}

MatrixXd NonlinearElastic::EMatrix(const double & modulus) const
{
    MatrixXd E(4,4);
    if (!anisotropy) {
        E << 1 - v, v, v, 0,
              v,   1-v, v, 0,
              v,   v,  1-v, 0,
              0,  0,    0,  (1-2*v)/2;
        E = E * modulus / (1+v) /(1-2*v);
    } else {
        double n = r_Mr;
        double m = r_G;
        double A = modulus / (1 + vr) / (1 - vr - 2 * n * vz * vz);
        E << n * (1 - n * vz * vz), n * (vr + n * vz * vz), n * vz * (1 + vr), 0,
              n * (vr + n * vz * vz), n * (1 - n * vz * vz), n * vz * (1 + vr), 0,
              n * vz * (1 + vr), n * vz * (1 + vr), 1 - vr * vr, 0,
              0, 0, 0, m * (1 + vr) * (1 - vr - 2 * n * vz * vz);
        E = E * A;
    }
    return E;
}
