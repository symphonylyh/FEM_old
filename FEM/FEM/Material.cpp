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

void Material::stressDependent(const double & bulk, const double & deviator) const
{
}

const Vector2d & Material::bodyForce() const
{
    return bodyForce_;
}

const VectorXd & Material::thermalStrain() const
{
    return thermalStrain_;
}
