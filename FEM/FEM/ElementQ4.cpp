/**
 * @file ElementQ4.cpp
 * Implementation of ElementQ4 class.
 *
 * @author Haohang Huang
 * @date Feburary 10, 2018
 */

#include "ElementQ4.h"

// static member should be initialized outside the class body because it doesn't
// depend on any instance of that class and is initialized before any instance is
// created. Initialization here does not require the static member to be public.
// ElementQ4::staticMembers ElementQ4::statics(4,4,4,2,2);

ElementQ4::ElementQ4()
{
}

ElementQ4::ElementQ4(const int & index, const std::vector<int> & nodeList, Node** const meshNode)
  : Element(index, nodeList, meshNode) // call the constructor of base class in the initializer list!
{
}

ElementQ4::~ElementQ4()
{
}

// Shape* const ElementQ4::shape() const
// {
//     return statics.shape;
// }
