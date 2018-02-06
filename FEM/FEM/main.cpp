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

int main() {
    /*
    Node node1(1.0,2.0, 1);
    node1.fixBoundary();
    node1.setDisp(1.0, 2.0);
    Vector2d vec2 = node1.getLocalCoord();
    std::cout << vec2(1) << std::endl;
    return 0;
     */
     Mesh mesh;
     mesh.dataCount("meshData.txt"); // in Xcode, mind the working directory, should Product-Scheme-Use custom working directory. Otherwise please use absolute path
     mesh.readFromFile("meshData.txt");

    return 0;
}
