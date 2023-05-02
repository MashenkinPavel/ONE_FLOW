//
// Created by pavel on 03.05.2023.
//
#pragma once

#include <vector>

template <class MType>
class SLAE {
public:
    SLAE(){
        elems.resize(0);
        col_ind.resize(0);
        shift.resize(0);
    }
    ~SLAE(){}
    void Add (int i, int j, MType val){

    }
private:
    std::vector<int> col_ind;
    std::vector<int> shift;
    std::vector<MType> elems;
};

