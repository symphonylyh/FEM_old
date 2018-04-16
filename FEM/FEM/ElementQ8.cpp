/**
 * @file ElementQ8.cpp
 * Implementation of ElementQ8 class.
 *
 * @author Haohang Huang
 * @date Feburary 13, 2018
 */

// for use of M_PI
#define _USE_MATH_DEFINES
#include <cmath>
#include "ElementQ8.h"

// static member should be initialized outside the class body because it doesn't
// depend on any instance of that class and is initialized before any instance is
// created. Initialization here does not require the static member to be public.
ElementQ8::staticMembers ElementQ8::statics(8,9,4,3,3);

ElementQ8::ElementQ8()
{
}

ElementQ8::ElementQ8(const int & index, const std::vector<int> & nodeList, Node** const meshNode)
    : Element(index, nodeList, meshNode) // call the constructor of base class in the initializer list!
{
}

ElementQ8::~ElementQ8()
{
}

Shape* const ElementQ8::shape() const
{
    return statics.shape;
}
