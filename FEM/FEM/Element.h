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
     * @param meshNode A const pointer to the node pool of the mesh.
     *
     * @note Old version of this function pass in all the nodes, which is expensive.
     * This revised version only pass in a pointer to access the node pool of the mesh.
     */
    Element(const int & index, const std::vector<int> & nodeList, Node* const meshNode);

    /**
     * Copy constructor.
     */
    Element(Element const & other);

    /**
     * Assignment operator.
     */
    Element const & operator=(Element const & other);

    /**
     * Destructor.
     */
    virtual ~Element();

    virtual MatrixXd localStiffness() = 0;
    virtual MatrixXd jacobian(const Vector2d & point) const = 0;
    virtual MatrixXd BMatrix(const Vector2d & gaussianPoint) const = 0;
    void computeEMatrix();
    void setPoissonRatio(double v);
    void setModulus(double E);

    /**
     * Custom constructor to create an element with given node indices.
     *
     * @param index The index number of current element.
     * @param nodeList The list of node indices this element is consist of.
     * @param meshNode A const pointer to the node pool of the mesh.
     *
     * @note Old version of this function pass in all the nodes, which is expensive.
     * This revised version only pass in a pointer to access the node pool of the mesh.
     */
    int getIndex() const;
    int getSize() const;
    VectorXi getNodeList() const;
    MatrixXd getNodeCoord() const;
    virtual Shape* getShape() const = 0;

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
