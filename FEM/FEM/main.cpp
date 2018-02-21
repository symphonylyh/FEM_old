//
//  main.cpp
//  FEM
//
//  Created by 黄浩航 on 04/02/2018.
//  Copyright © 2018 HHH. All rights reserved.
//

#include "Element.h"
#include "Node.h"
#include "Mesh.h"
#include "Eigen/Eigen"
#include <iostream>
#include <fstream>
#include <string>

using namespace Eigen;

#include "IO.h"

int main() {
    IOFormat CleanFmt(4, 0, ", ", "\n", "[", "]");
    /* Test Node.h and Mesh.h
    Mesh mesh;
    mesh.dataCount("meshData.txt"); // in Xcode, mind the working directory, should Product-Scheme-Use custom working directory. Otherwise please use absolute path
    mesh.readFromFile("meshData.txt");
    MatrixXd nodeCoord0 = mesh.getNodeCoord(0);
    MatrixXi elementIndex8 = mesh.getElementIndex(8);

    Node node0 = mesh.getNode(0);
    node0.setBoundaryAll();
    node0.setDisp(1.5,1.6);
    std::cout << node0.getDisp() << std::endl;
    std::cout << nodeCoord0 << std::endl;
    */

    /* Test Shape.h and the Shape class's polymorphism of inheritance (Q4, Q8)
    ShapeQ4 Q4;
    ShapeQ8 Q8;
    Shape* shapeList[3]; // create a list of shape ptr that can contain three different types of shape
    shapeList[0] = &Q4; // or new ShapeQ4();
    shapeList[1] = &Q8;
    Vector2d point(0.5,0.5);
    MatrixXd N_Q4 = shapeList[0]->shapeFunction(point);
    MatrixXd dN_Q4 = shapeList[0]->shapeLocalDeriv(point);

    IOFormat CleanFmt(4, 0, ", ", "\n", "[", "]");

    std::cout << "N (Q4): \n" << N_Q4.format(CleanFmt) << std::endl;
    std::cout << "Size of N (Q4): " << N_Q4.rows() << "x" << N_Q4.cols() << std::endl;
    std::cout << "dN (Q4): \n" << dN_Q4.format(CleanFmt) << std::endl;
    std::cout << "Size of dN (Q4):" << dN_Q4.rows() << "x" << dN_Q4.cols() << std::endl;

    MatrixXd N_Q8 = shapeList[1]->shapeFunction(point);
    MatrixXd dN_Q8 = shapeList[1]->shapeLocalDeriv(point);
    std::cout << "N (Q8): \n" << N_Q8.format(CleanFmt) << std::endl;
    std::cout << "Size of N (Q8): " << N_Q8.rows() << "x" << N_Q8.cols() << std::endl; // expected 2x16
    std::cout << "dN (Q8): \n" << dN_Q8.format(CleanFmt) << std::endl;
    std::cout << "Size of dN (Q8):" << dN_Q8.rows() << "x" << dN_Q8.cols() << std::endl; // expected 2x8
    */

    /* Test Element.h and ElementQ4.h
    Mesh mesh;
    mesh.dataCount("meshData.txt"); // in Xcode, mind the working directory, should Product-Scheme-Use custom working directory. Otherwise please use absolute path
    mesh.readFromFile("meshData.txt");
    Element* element8 = mesh.getElement(8);
    std::cout << element8->printNodeList() << std::endl;
    */

    /* Test assebleStiffness() in Mesh.cpp (Jiayi)
    Mesh mesh;
    mesh.dataCount("meshData.txt");
    mesh.readFromFile("meshData.txt");
    std::cout << mesh.assembleStiffness().format(CleanFmt) << std::endl;
    */
    return 0;
}
