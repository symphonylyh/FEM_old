/**
 * @file Element.h
 * Abstract base class for various isoparametric elements.
 *
 * @author Haohang Huang
 * @date Feburary 4, 2018
 */

#ifndef Element_h
#define Element_h

#include "Node.h"
#include "Shape.h"

/* Abstract base Element class with shared methods and pure virtual methods.
 *
 * For polymorphism of a combination of different element types (e.g., T3, Q6, Q8), we
 * create a base class with virtual methods and enable different implementation
 * using C++ inheritance feature. Methods can be accessed using a generic
 * pointer Element*.
 */
class Element
{
  public:
    /**
     * Default constructor for Element.
     */
    Element();

    /**
     * Custom constructor to create an element with given node indices.
     *
     * @param index The index number of current element.
     * @param nodeList The list of node indices this element is consist of.
     * @param meshNode A pointer to the node pool of the mesh.
     *
     * @note Old version of this function pass in all the nodes, which is expensive.
     * This revised version only pass in a pointer to access the node pool of the mesh.
     */
    Element(const int & index, const std::vector<int> & nodeList, Node const * & meshNode);

    // Big Three
    Element(Element const & other);
    Element const & operator=(Element const & other);
    virtual ~Element();

    int getIndex() const;
    void setIndex(int index);
    int getSize() const;
    VectorXi getNodeList() const;
    MatrixXd getNodeCoord() const;
    virtual MatrixXd localStiffness() = 0;
    virtual MatrixXd jacobian(const Vector2d & point) const = 0;
    virtual MatrixXd BMatrix(const Vector2d & gaussianPoint) const = 0;
    void computeEMatrix();

    virtual Shape* getShape() const = 0;

    void setPoissonRatio(double v);
    void setModulus(double E);

    double poissonRatio;
    double modulus;
    MatrixXd E;

  private:
    int index_;
    int size_;
    VectorXi nodeList_;
    MatrixXd nodeCoord_;

    // Helper functions
    void clear_();
    void copy_(Element const & other);


};
#endif /* Element_h */
