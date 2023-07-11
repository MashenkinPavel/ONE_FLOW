#include <iostream>
#include "mesh.h"
#include "SLAE.h"

int main(int argc, char* argv[]){
    std::cout<<"Program for calculating CFD\n";
    std::cout<<"made by Mashenkin Pavel\n";

    {
        Mesh mesh;
        mesh.Read_msh_file("../test_data/mesh.msh");
        mesh.Write_vtk_file("../test_data/mesh.vtk");
    }

    {
        Mesh mesh;
        mesh.Read_msh_file("./test_data/mesh.msh");
        mesh.Write_vtk_file("./test_data/mesh.vtk");
    }

    {
        Mesh mesh;
        mesh.Read_msh_file("./test_data/mesh_1tetr.msh");
        mesh.Write_vtk_file("./test_data/mesh_1tetr.vtk");
    }

    {
        Mesh mesh;
        mesh.Read_msh_file("./test_data/mesh_2tetr.msh");
        mesh.Write_vtk_file("./test_data/mesh_2tetr.vtk");
    }


    {
        testfunc1(argc,argv);
    }




    return 0;
}
