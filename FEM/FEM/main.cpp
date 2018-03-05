//
//  main.cpp
//  FEM
//
//  Created by 黄浩航 on 04/02/2018.
//  Copyright © 2018 HHH. All rights reserved.
//

// #include "Element.h"
// #include "Node.h"
// #include "Mesh.h"
//#include "Eigen/Eigen"
#include <iostream>
#include <fstream>
#include <string>

#include "LinearElastic.h"
#include "IO.h"


//using namespace Eigen;

int main() {
    IOFormat CleanFmt(4, 0, ", ", "\n", "[", "]");
    /* Test Node.h and Mesh.h
    Mesh mesh;
    mesh.dataCount("meshData.txt"); // in Xcode, mind the working directory, should Product-Scheme-Use custom working directory. Otherwise please use absolute path
    mesh.readFromFile("meshData.txt");
    Element* element8 = mesh.getElement(8);
    std::cout << element8->getNodeList().format(CleanFmt) << std::endl;
    std::cout << element8->getNodeCoord().format(CleanFmt) << std::endl;

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
    MatrixXd N_Q4 = shapeList[0]->function(point);
    MatrixXd dN_Q4 = shapeList[0]->localDeriv(point);

    IOFormat CleanFmt(4, 0, ", ", "\n", "[", "]");

    std::cout << "N (Q4): \n" << N_Q4.format(CleanFmt) << std::endl;
    std::cout << "Size of N (Q4): " << N_Q4.rows() << "x" << N_Q4.cols() << std::endl;
    std::cout << "dN (Q4): \n" << dN_Q4.format(CleanFmt) << std::endl;
    std::cout << "Size of dN (Q4):" << dN_Q4.rows() << "x" << dN_Q4.cols() << std::endl;

    MatrixXd N_Q8 = shapeList[1]->function(point);
    MatrixXd dN_Q8 = shapeList[1]->localDeriv(point);
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

    /*
    ShapeQ8 shapeQ8;
    std::vector<Vector2d> gPointQ8 = shapeQ8.gaussianPoint();
    std::vector<double> gWeightQ8 = shapeQ8.gaussianWeight();
    std::cout << "Gaussian Points:" << std::endl;
    for (std::vector<Vector2d>::iterator i = gPointQ8.begin(); i != gPointQ8.end(); i++) {
      std::cout << *i << "\n" << std::endl;
    }
    std::cout << "Gaussian Weights:" << std::endl;
    for (std::vector<double>::iterator i = gWeightQ8.begin(); i != gWeightQ8.end(); i++) {
      std::cout << *i << "\n" << std::endl;
    }
    */

    /* Test ShapeQ8, ElementQ8 of computing local stiffness matrix
    Mesh mesh;
    mesh.dataCount("meshData.txt");
    mesh.readFromFile("meshData.txt");

    Element* element8 = mesh.getElement(8);
    std::cout << "Local stiffness matrix of element No.8: " << std::endl;
    std::cout << "Size of local stiffness matrix: " << element8->localStiffness().rows() << "x" << element8->localStiffness().cols() << std::endl;
    std::cout << element8->localStiffness().format(CleanFmt) << std::endl;
    */

    /* Test global stiffness matrix assembly
    Mesh mesh;
    mesh.dataCount("meshData.txt");
    mesh.readFromFile("meshData.txt");
    std::cout << "Global stiffness matrix: \n" << "Size: " << mesh.assembleStiffness().rows() << "x" << mesh.assembleStiffness().cols() << std::endl;
    std::cout << MatrixXd(mesh.assembleStiffness()).format(CleanFmt) << std::endl;
    */
    // Above this line is before Analysis{.h, .cpp} is added
    //-------------------------------------------------------------------------
    // Test Analysis.h and LinearElastic.h
    Analysis* case1 = new LinearElastic("meshData.txt");
    MatrixXd A = MatrixXd(case1->getGlobalStiffness());
    std::cout << "Global stiffness matrix: \n" << "Size: " << A.rows() << "x" << A.cols() << std::endl;
    std::cout << A.format(CleanFmt) << std::endl;
    std::cout << case1->getMesh().getElement(0)->localStiffness().format(CleanFmt) << std::endl;
    JacobiSVD<MatrixXd> svd(A);
    double cond = svd.singularValues()(0) / svd.singularValues()(svd.singularValues().size()-1);
    std::cout << "Condition number: " << cond << std::endl;
    std::cout << "Determinant: " << A.determinant() << std::endl;
    FullPivLU<MatrixXd> lu_decomp(A);
    std::cout << "The rank of A is: " << lu_decomp.rank() << std::endl;

    //Jiayi
    std::cout << "Modify the stiffness matrix: " << std::endl;
    case1->modifiedStiffness_x();
    case1->modifiedStiffness_y();
    A = MatrixXd(case1->getGlobalStiffness());
    std::cout << A.format(CleanFmt) << std::endl;
    JacobiSVD<MatrixXd> svd2(A);
    double cond2 = svd2.singularValues()(0) / svd2.singularValues()(svd2.singularValues().size()-1);
    std::cout << "Condition number: " << cond2 << std::endl;
    std::cout << "Determinant: " << A.determinant() << std::endl;
    FullPivLU<MatrixXd> lu_decomp2(A);
    std::cout << "The rank of A is: " << lu_decomp2.rank() << std::endl;

    VectorXd b = case1->assembleAppliedForce();
    VectorXd x;

    //SimplicialLDLT <SparseMatrix<double> > solver;
    ConjugateGradient <SparseMatrix<double> > solver;
    // SimplicialLDLT is direct solver: Recommended for very sparse and not too large problems (e.g., 2D Poisson eq.)
    // ConjugateGradient is iterative solver: Recommended for large symmetric problems (e.g., 3D Poisson eq.)
    solver.compute(case1->getGlobalStiffness());
    x = solver.solve(b);

    std::cout <<x<< std::endl;


    delete case1; case1 = NULL;

    return 0;
}
