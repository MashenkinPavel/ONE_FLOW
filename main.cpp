#include <iostream>
#include "mesh.h"

int main(int argc, char* argv[]){
    std::cout<<"Program for calculating CFD\n";
    std::cout<<"made by Mashenkin Pavel\n";

    Mesh mesh;
    mesh.Read_msh_file("./test_data/mesh.msh");
    //mesh.Read_msh_file("./test_data/mesh_1tetr.msh");
    //mesh.Read_msh_file("./test_data/mesh_2tetr.msh");
    mesh.Write_vtk_file("./test_data/my_vers_mesh.vtk");

    return 0;
}