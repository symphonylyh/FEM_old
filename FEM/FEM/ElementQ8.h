/**
 * @file ElementQ8.h
 * Derived class from Element for the isoparametric Q8 element.
 *
 * @author Haohang Huang
 * @date Feburary 13, 2018
 * @note Efficiency optimized by polymorph shape on March 26, 2018
 */

#ifndef ElementQ8_h
#define ElementQ8_h

#include "Element.h"
#include "ShapeQ8.h"

/* Derived class for the isoparametric Q8 element.
 */
class ElementQ8 : public Element
{
    public:
        /* See the documentation of base class Element. */
        ElementQ8();
        ElementQ8(const int & index, const std::vector<int> & nodeList, Node** const meshNode);
        ~ElementQ8();

        Shape* const shape() const;

    private:
        /** A static structure that manages all the static members used in this class */
        static staticMembers statics;
};

#endif /* ElementQ8_h */
