//
//  main.cpp
//  FEM
//
//  Created by 黄浩航 on 04/02/2018.
//  Copyright © 2018 HHH. All rights reserved.
//

#include "Element.h"
#include "Node.h"
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
    
    int nodeCount = 0;
    int elementCount = 0;
    
    std::ifstream inFile("meshData.txt"); // in Xcode, mind the working directory, should Product-Scheme-Use custom working directory. Otherwise please use absolute path
    
    if(!inFile) {
        std::cout << "Cannot open input file." << std::endl;
        return 1;
    }
    
    // Count numbers
    std::string byLine;
    while(!inFile.eof()) {
        std::getline(inFile, byLine);
        std::stringstream oneLine(byLine);
        if (byLine[0] == 'v') {
            nodeCount++;
        } else if (byLine[0] == 'f') {
            elementCount++;
        }
    }
    inFile.close();
    
    MatrixXd nodeCoord(nodeCount,2);
    MatrixXi elementIndex(elementCount,4);
    int i = 0;
    int j = 0;
    char c;
    std::ifstream inFile2("meshData.txt");
    std::string byLine2;
    while(!inFile2.eof()) {
        std::getline(inFile2, byLine2);
        std::stringstream oneLine2(byLine2);
        if (byLine2[0] == 'v') {
            oneLine2 >> c >> nodeCoord(i,0) >> nodeCoord(i,1); // ">>" is by default separated by space
            i++;
        } else if (byLine2[0] == 'f') {
            oneLine2 >> c >> elementIndex(j,0) >> elementIndex(j,1) >> elementIndex(j,2) >> elementIndex(j,3);
            j++;
        }
    }
    inFile2.close();
    std::cout << nodeCoord << std::endl;
    std::cout << elementIndex << std::endl;

    return 0;
}
