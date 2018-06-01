/**
 * @file Material.cpp
 * Implementation of the Material class.
 *
 * @author Haohang Huang
 * @date Apr 27, 2018
 */

#include "Material.h"

Material::Material(const bool & Anisotropy, const bool & Nonlinearity)
  : anisotropy(Anisotropy), nonlinearity(Nonlinearity), modulus_(0), E_(MatrixXd::Zero(4,4)), thermalStrain_(VectorXd::Zero(4))
{
}

Material::~Material()
{
}

const double & Material::modulus() const
{
    return modulus_;
}

const MatrixXd & Material::EMatrix() const
{
    return E_;
}

double Material::stressDependentModulus(const VectorXd & stress) const
{
    return 0; // to silent warning
}

MatrixXd Material::EMatrix(const double & modulus) const
{
    return MatrixXd::Zero(4,4); // to silent warning
}

const Vector2d & Material::bodyForce() const
{
    return bodyForce_;
}

const VectorXd & Material::thermalStrain() const
{
    return thermalStrain_;
}
