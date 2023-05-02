//
// Created by pavel on 18.01.2023.
//
#pragma once
#include <vector>

#include "face.h"
#include "node.h"
#include "Rib.h"

#include <set>
#include <map>
class Cell {
private:
    std::vector<int> mFaces; //ячейка знает инфу по своим граням

public:
    std::vector<int> CellGetFaceList();
    void CellSetFaceList(std::vector<int> facelist){
        mFaces = facelist;
    }




};


