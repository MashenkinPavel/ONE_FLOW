//
// Created by pavel on 18.01.2023.
//

#pragma once


#include <vector>
#include "node.h"
#include <map>


class Face {
private:
    std::vector<int> rib_list; //список индексов ребер из которых состоит грань
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
    std::vector<int> GetNodeList();
    void AddRib(const std::map<std::pair<int,int>,int>& mapNdsPairNumberToRibNum,int nodeInd1, int nodeInd2);
};
