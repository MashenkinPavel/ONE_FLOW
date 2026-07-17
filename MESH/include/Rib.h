//
// Created by pavel on 23.01.2023.
//

#ifndef ONE_FLOW_RIB_H
#define ONE_FLOW_RIB_H
#include <vector>


class Rib {
private:
    int vertex1;
    int vertex2;
public:
    Rib(int v1 = -1 ,int v2 = -1):
    vertex1(v1),vertex2(v2){

    }
    void SetRibNodes(const int v1,const int v2){
        vertex1 = v1;
        vertex2 = v2;
    }
    void SetRibNodes(std::pair<int,int> v){
        vertex1 = v.first;
        vertex2 = v.second;
    }

    void GetRibNodes(int& v1, int& v2){
        v1 = vertex1;
        v2 = vertex2;
    }
    ~Rib()=default;
};


#endif //ONE_FLOW_RIB_H
