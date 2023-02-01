//
// Created by pavel on 18.01.2023.
//

#ifndef ONE_FLOW_CELL_H
#define ONE_FLOW_CELL_H
#include <vector>

#include "face.h"
#include "node.h"
#include "Rib.h"

#include <set>
#include <map>
class Cell {
private:
    //std::vector<int> mNodes;
    //std::vector<int> mRibs;
    std::vector<int> mFaces; //ячейка знает инфу по своим граням

public:
    std::vector<int> CellGetFaceList();
    void CellSetFaceList(std::vector<int> facelist){
        mFaces = facelist;
    }




};


#endif //ONE_FLOW_CELL_H
