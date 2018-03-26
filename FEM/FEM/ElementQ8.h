/**
 * @file ElementQ8.h
 * Derived class from Element for the isoparametric Q8 element.
 *
 * @author Haohang Huang
 * @date Feburary 13, 2018
 */

#ifndef ElementQ8_h
#define ElementQ8_h

#include "Element.h"
#include "ShapeQ8.h"

class ElementQ8 : public Element
{
  private:
    struct staticMembers {
          Shape* shape;
          // ctor
          staticMembers() {
              shape = new ShapeQ8;
          }
          // dtor
          ~staticMembers() {
              delete shape; shape = NULL;
          }
      };

  public:
    ElementQ8();
    ElementQ8(const int & index, const std::vector<int> & nodeList, const Node* meshNode);
    ~ElementQ8();

    MatrixXd localStiffness();
    MatrixXd jacobian(const Vector2d & point) const;

    MatrixXd BMatrix(const Vector2d & point) const;

    //static ShapeQ8 shape;

    static staticMembers statics;

    Shape* getShape() const;

  private:
    MatrixXd termsAtGaussianPt(const Vector2d & point, MatrixXd & nodeCoord);
};

#endif /* ElementQ8_h */
