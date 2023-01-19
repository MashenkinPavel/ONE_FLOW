//
// Created by pavel on 18.01.2023.
//

#ifndef ONE_FLOW_FACE_H
#define ONE_FLOW_FACE_H


#include <vector>
#include "node.h"

class Face {
private:
    std::vector<int> node_list; // nodes list of nodes numbers in face
    int left_cell;              //index of left cell
    int right_cell;             //index of right cell

    //cell stencil for approximation maybe store?

public:
    Face();
    ~Face();
    void SetLCell(int lcell){
        left_cell = lcell;
    }
    void SetRCell(int rcell){
        right_cell = rcell;
    }
    void AddNode(int nodenumber);

};


#endif //ONE_FLOW_FACE_H
