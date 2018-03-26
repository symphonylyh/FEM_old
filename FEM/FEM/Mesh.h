/**
 * @file Mesh.h
 * Mesh class for the node, element, material information from input file.
 *
 * @author Haohang Huang
 * @date Feburary 5, 2018
 */

#ifndef Mesh_h
#define Mesh_h

#include "Node.h"
#include "Element.h"
#include <vector>

class Mesh
{
    public:
        Mesh(std::string const & fileName);
        bool dataCount(std::string const & fileName); // record the No. of nodes and elements in the mesh
        bool readFromFile(std::string const & fileName); // read the mesh info and store in node & element object
        // Different from Node.h, here we return a reference because the object meshNode_ & meshElement_ are on HEAP!
        Node* & getNode(int index) const; // get the node object at given 0-based index
        Element* & getElement(int index) const; // get the element object at given 0-based index
        int nodeCount() const;
        int elementCount() const;
        Node** nodeArray() const;
        Element** elementArray() const;
        // MatrixXd queryPoint(double x, double y); // interpolate disp, stress, strain at query point, should call Shape method, to be implemented...
        //SparseMatrix<double> assembleStiffness() const;
        ~Mesh();

        std::vector<int> boundaryNodeList; // actually this is not NODE list, but degree of freedom list
        std::vector<double> boundaryValue;
        std::vector<int> loadNodeList;
        std::vector<double> loadValue;

    private:
        int nodeCount_;
        int elementCount_;
        Node** meshNode_; // previously meshNode_ is a arr<Node>, but when I do meshNode_ = new Node[size]; meshNode_[i] = Node(...), actually the Node ctor is first called and a copy assignment operator is used. This is of low efficiency
        Element** meshElement_;

};

#endif /* Mesh_h */
