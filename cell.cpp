//
// Created by pavel on 18.01.2023.
//

#include "cell.h"
#include <vector>

const std::vector<int>& Cell::CellGetFaceList() const {
    return mFaces;
}
