/**
 * @file NonlinearElastic.cpp
 * Implementation of NonlinearElastic class.
 *
 * @author Haohang Huang
 * @date May 19, 2018
 */

#include "NonlinearElastic.h"

NonlinearElastic::NonlinearElastic(const bool & anisotropy, const bool & nonlinearity, const std::vector<double> & properties)
  : Material(anisotropy, nonlinearity)
{
    int i = 0;
    // Uzan (1985) model:
    // M = k1 * theta^k2 * sigma_d^k3 where theta = sigma1 + sigma2 + sigma3 (bulk stress), sigma_d = sigma1 - sigma3 (deviatoric stress)
    double k1 = properties[i++];
    double k2 = properties[i++];
    double k3 = properties[i++];
    double M0 = properties[i++]; // initial guess of resilient modulus

    v = properties[i++];
    coeffUzan << k1, k2, k3; // the model parameters should be cached to be used in modulus update during nonlinear scheme

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

void NonlinearElastic::stressDependent(const double & bulk, const double & deviator) const
{

}
