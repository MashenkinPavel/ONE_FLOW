//
// Created by pavel on 18.01.2023.
//

#ifndef ONE_FLOW_NODE_H
#define ONE_FLOW_NODE_H

class Node {
  private:
      double x;
      double y;
      double z;
  public:
    Node()
    :x(0.0),y(0.0),z(0.0){

    }
    Node (double xx, double yy, double zz):x(xx),y(yy),z(zz){

    }
    ~Node(){
    }

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

#endif //ONE_FLOW_NODE_H
