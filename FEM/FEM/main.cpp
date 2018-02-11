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
#include "ShapeQ4.h"
#include "Eigen/Eigen"
#include <iostream>
#include <fstream>
#include <string>

using namespace Eigen;

int main() {
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
    Element element8 = mesh.getElement(8);
    std::cout << element8.getIndex() << std::endl;
    std::cout << nodeCoord0 << std::endl;
    std::cout << elementIndex8 << std::endl;
    */

    /* Test Shape.h and the Shape class's polymorphism of inheritance
    ShapeQ4 Q4;
    Shape* shapeList[3]; // create a list of shape ptr that can contain three different types of shape
    shapeList[0] = &Q4; // or new ShapeQ4();
    Vector2d point(0.5,0.5);
    MatrixXd N = shapeList[0]->shapeFunction(point);
    MatrixXd dN = shapeList[0]->shapeLocalDeriv(point);
    std::cout << "N: " << N << std::endl;
    std::cout << "dN:" << dN << std::endl;
    */


    return 0;
}
