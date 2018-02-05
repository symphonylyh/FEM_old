//
//  Node.h
//  FEM
//
//  Created by 黄浩航 on 04/02/2018.
//  Copyright © 2018 HHH. All rights reserved.
//

#ifndef Node_h
#define Node_h

class Node {
    
public:
    Node();
    ~Node();
    void setDisplacement(double u, double v);
    double getDisplacement();
    
private:
    double u_; // displacement in 2D
    double v_;
    
};

#endif /* Node_h */
