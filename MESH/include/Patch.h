#pragma once
#include <string>
#include <functional>
#include <memory>

#include <BC.h>

class Patch{
enum class PatchType{
    Default,
    Inner,
    Patch,
    Wall,
    Empty
};

public:
Patch() = default;
~Patch() = default;
void set_name(const std::string& patch_name){
    m_name = patch_name;
}
std::string get_name()const {
    return m_name;
}
void set_type(const std::string& type_name){
    if (type_name == "patch"){
        m_patchtype = PatchType::Patch;
    }else if (type_name == "wall"){
        m_patchtype = PatchType::Wall;
    }else{
        m_patchtype = PatchType::Empty;
    }
}
void set_nFaces(int nfaces){
    m_facecount = nfaces;
}

int get_nFaces()const {
    return m_facecount;
}

void set_startFace(int startFace){
    m_startface = startFace;
}
int get_startFace()const{
    return m_startface;
}

private:
    PatchType m_patchtype;
    std::string m_name;
    int m_startface = -1;
    int m_facecount = -1; 
    std::shared_ptr<BC> sptr_bc;
    
    std::function<double(double x, double y, double z, double t)>  m_func;

};
