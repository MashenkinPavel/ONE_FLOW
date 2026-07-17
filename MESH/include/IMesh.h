#pragma once
#include <string>


class IMesh{
public:
    virtual bool ReadMesh(const std::string &filename)  = 0;
    virtual void WriteMesh(const std::string &filename) = 0;
    virtual ~IMesh()=default;
};

