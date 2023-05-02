//
// Created by pavel on 18.01.2023.
//
#pragma once

#include <vector>

class Node {
  private:
      double x;
      double y;
      double z;
      std::vector<int> neib_nodes; //соседи к узлу через ребра
      std::vector<int> neib_cells; //соседние к узлу ячейки

  public:
    Node()
    :x(0.0),y(0.0),z(0.0){

    }
    Node (double xx, double yy, double zz):x(xx),y(yy),z(zz){

    }
    ~Node() = default;

    void SetX(double x);
    void SetY(double y);
    void SetZ(double z);

    double GetX(){
        return x;
    }
    double GetY(){
        return y;
    }
    double GetZ(){
        return z;
    }
};

