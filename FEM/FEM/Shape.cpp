/**
 * @file Shape.cpp
 * Implementation of Shape class.
 *
 * @author Haohang Huang
 * @date Feburary 10, 2018
 */

#include "Shape.h"
#include <cmath>

Shape::Shape(int nodes, int gaussians, int edges, int edgeNodes, int edgeGaussians) :
// use initializer list to define the size of vector!
    numNodes_(nodes), nodeCoord_(nodes),
    numGaussianPts_(gaussians), gaussianPt_(gaussians), gaussianWt_(gaussians),
    shapeVec_(gaussians), shapeMat_(gaussians), shapeDeriv_(gaussians),
    numEdges_(edges), numEdgeNodes_(edgeNodes), edgeList_(edges),
    numEdgeGaussianPts_(edgeGaussians), edgeGaussianPt_(edgeGaussians), edgeGaussianWt_(edgeGaussians),
    edgeShapeVec_(edgeGaussians), edgeShapeMat_(edgeGaussians), edgeShapeDeriv_(edgeGaussians)
{
    // Do nothing, only specify the size of each member variable. Computation
    // are done in the constructor of each derived class.
}

const VectorXd & Shape::functionVec(int i) const
{
    return shapeVec_[i];
}

const MatrixXd & Shape::functionMat(int i) const
{
    return shapeMat_[i];
}

const MatrixXd & Shape::functionDeriv(int i) const
{
    return shapeDeriv_[i];
}

const VectorXd & Shape::edgeFunctionVec(int i) const
{
    return edgeShapeVec_[i];
}

const MatrixXd & Shape::edgeFunctionMat(int i) const
{
    return edgeShapeMat_[i];
}

const VectorXd & Shape::edgeFunctionDeriv(int i) const
{
    return edgeShapeDeriv_[i];
}

void Shape::_cacheShape()
{
    for (int g = 0; g < numGaussianPts_; g++) {
        shapeVec_.push_back(functionVec(gaussianPt_[g]));
        shapeMat_.push_back(functionMat(gaussianPt_[g]));
        shapeDeriv_.push_back(functionDeriv(gaussianPt_[g]));
    }

    for (int n = 0; n < numEdgeGaussianPts_; n++) {
        edgeShapeVec_.push_back(edgeFunctionVec(edgeGaussianPt_[n]));
        edgeShapeMat_.push_back(edgeFunctionMat(edgeGaussianPt_[n]));
        edgeShapeDeriv_.push_back(edgeFunctionDeriv(edgeGaussianPt_[n]));
    }
}
