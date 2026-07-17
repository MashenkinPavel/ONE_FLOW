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

const std::vector<int>& Face::GetNodeList() const {
    return node_list;
}

void Face::AddRib(const std::map< std::pair<int, int>, int >& mapNdsPairNumberToRibNum, int nodeInd1, int nodeInd2) {
    if (nodeInd1>nodeInd2) std::swap(nodeInd1, nodeInd2);
    std::pair<int,int> min_max = {nodeInd1,nodeInd2};
    int rib_number = mapNdsPairNumberToRibNum.at(min_max);
    rib_list.push_back( rib_number );
}

//void Face::GetNodeList(std::vector<int> &node_arr) {
//    node_arr = node_list;//node_list
//}
