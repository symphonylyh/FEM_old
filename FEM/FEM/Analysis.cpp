//
//  Analysis.cpp
//  FEM
//
//  Created by 黄浩航 on 26/02/2018.
//  Copyright © 2018 HHH. All rights reserved.
//

#include "Analysis.h"

Analysis::Analysis(std::string const & fileName) : mesh_(fileName), globalStiffness_(assembleStiffness())
{
}

Analysis::~Analysis()
{
    delete globalStiffness_; globalStiffness_ = NULL;
}

SparseMatrix<double>* Analysis::assembleStiffness()
{
    // initialize sparse matrix
    globalStiffness_ = new SparseMatrix<double>(2 * mesh_.nodeCount(),2 * mesh_.nodeCount());
    globalStiffness_->setZero();

    for (int i = 0; i < mesh_.elementCount(); i++) {
        Element* curr = mesh_.elementArray()[i];
        int size = curr->getSize();// element type, for Q4 element, size=4; for Q8, size=8, etc
        MatrixXi nodeList = curr->getNodeList();// the index of nodes belong to this element, e.g., for element8, it will give you a vector contain (10,11,15,14), use this for your globalStiffness matrix's location
        MatrixXd localStiffness = curr->localStiffness();
        for (int j = 0; j < size; j++){
            for (int k = 0; k < size; k++){
                globalStiffness_->coeffRef(2 * nodeList(j), 2 * nodeList(k)) += localStiffness(2 * j , 2 * k);
                globalStiffness_->coeffRef(2 * nodeList(j) + 1, 2 * nodeList(k) + 1) += localStiffness(2 * j + 1, 2 * k + 1);
            }
        }
    }
    globalStiffness_->makeCompressed();
    return globalStiffness_;
}

Mesh& Analysis::getMesh()
{
    return mesh_;
}

SparseMatrix<double>& Analysis::getGlobalStiffness()
{
    return *globalStiffness_;
}
