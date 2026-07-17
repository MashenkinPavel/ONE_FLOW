//
// Created by pavel on 18.01.2023.
//

#pragma once


#include <vector>
#include "node.h"
#include <map>
#include <string>


class Face {
private:
    std::string BC_tag;   //( inner || symmetry || from_su2_file)
    std::vector<int> rib_list; //список индексов ребер из которых состоит грань
    std::vector<int> node_list; // nodes list of nodes numbers in face
    int left_cell=-1;              //index of left cell
    int right_cell=-1;             //index of right cell

    //cell stencil for approximation maybe store?

public:
    Face();
    ~Face();
    void SetBC_tag(const std::string& str){
        BC_tag = str;
    }
    const std::string& GetBC_tag()const{
        return BC_tag;
    }


    void SetLCell(int lcell){
        left_cell = lcell;
    }
    int GetLCell()const{
        return left_cell;
    }
    void SetRCell(int rcell){
        right_cell = rcell;
    }
    int GetRCell()const{
        return right_cell;
    }

    void AddNode(int nodenumber);
    const std::vector<int>& GetNodeList() const ;
    void AddRib(const std::map<std::pair<int,int>,int>& mapNdsPairNumberToRibNum,int nodeInd1, int nodeInd2);
};
