//
// Created by pavel on 18.01.2023.
//

#include "face.h"

void Face::AddNode(int nodenumber) {
    node_list.push_back(nodenumber);
}

Face::Face() {
    node_list.clear();
    left_cell = -1;
    right_cell = -1;
}

Face::~Face() {
    node_list.clear();
}
