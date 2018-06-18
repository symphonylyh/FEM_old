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
NonlinearElastic::NonlinearElastic(const bool & anisotropy, const bool & nonlinearity, const bool & noTension, const std::vector<double> & properties)
  : Material(anisotropy, nonlinearity, noTension)
{
    int i = 0;
    // Uzan (1985) model:
    // M = k1 * theta^k2 * sigma_d^k3 where theta = sigma1 + sigma2 + sigma3 (bulk stress), sigma_d = sigma1 - sigma3 (deviatoric stress)
    double k1 = properties[i++];
    double k2 = properties[i++];
    double k3 = properties[i++];
    double M0 = properties[i++]; // initial guess of resilient modulus

    modulus_ = M0;
    coeffUzan << k1, k2, k3; // the model parameters should be cached to be used in modulus update during nonlinear scheme
    v = properties[i++];

    // Initialize modulus and E matrix
    E_ << 1 - v, v, v, 0,
          v,   1-v, v, 0,
          v,   v,  1-v, 0,
          0,  0,    0,  (1-2*v)/2;
    E_ = E_ * M0 / (1+v) /(1-2*v);

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
    double bulk = stress(0) + stress(1) + stress(2);
    double deviator = stress(2) - stress(0); // sigma1 - sigma3

    // Calculated new stress-dependent resilient modulus from Uzan model
    double M = coeffUzan(0) * std::pow(std::abs(bulk), coeffUzan(1)) * std::pow(std::abs(deviator), coeffUzan(2));
    return M;
}

MatrixXd NonlinearElastic::EMatrix(const double & modulus) const
{
    MatrixXd E(4,4);
    E << 1 - v, v, v, 0,
          v,   1-v, v, 0,
          v,   v,  1-v, 0,
          0,  0,    0,  (1-2*v)/2;
    E = E * modulus / (1+v) /(1-2*v);
    return E;
}
