/**
 * @file ElementQ4.h
 * Derived class from Element for the isoparametric Q4 element.
 *
 * @author Haohang Huang
 * @date Feburary 10, 2018
 */

#ifndef ElementQ4_h
#define ElementQ4_h

#include "Element.h"
#include "ShapeQ4.h"

/* Derived class for the isoparametric Q4 element.
 */
class ElementQ4 : public Element
{
    public:
        /* See the documentation of base class Shape.
         */
        ElementQ4();
        ElementQ4(const int & index, const std::vector<int> & nodeList, Node** const meshNode);
        ~ElementQ4();
        MatrixXd localStiffness() const;
        MatrixXd jacobian() const;
        MatrixXd BMatrix() const;
        Shape* const getShape() const;
    private:


};

#endif /* ElementQ4_h */
