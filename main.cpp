#include <iostream>
#include "mesh.h"
#include "SLAE.h"
#include <exception>

int test_mesh_reading(){
{
        Mesh mesh;
        mesh.Read_msh_file("../test_data/mesh.msh");
        mesh.Write_vtk_file("../test_data/mesh.vtk");
    }

    {
        Mesh mesh;
        mesh.Read_msh_file("../test_data/mesh.msh");
        mesh.Write_vtk_file("../test_data/mesh.vtk");
    }

    {
        Mesh mesh;
        mesh.Read_msh_file("../test_data/mesh_1tetr.msh");
        mesh.Write_vtk_file("../test_data/mesh_1tetr.vtk");
    }

    {
        Mesh mesh;
        mesh.Read_msh_file("../test_data/mesh_2tetr.msh");
        mesh.Write_vtk_file("../test_data/mesh_2tetr.vtk");
    }

    {
        Mesh mesh;
        if (mesh.Read_SU2_file("../test_data/mesh_NACA0012_5deg_6814.su2")){
            printf("OK: tes SU2 file read compligte.\n");
        }else{
            throw std::runtime_error("can't read SU2 mesh testfile");
        }
    }
    

    try {
        Mesh mesh;
        if (mesh.Read_SU2_file("../test_data/mesh_NACA0012_5deg_6814.su2_error")){
            printf("SU2 file read OK!\n");
        }else{
            throw std::runtime_error("can't read SU2 mesh testfile");
        }
    }
    catch(...){
        printf("OK:    test on  problem finding file complite.\n");
    }

}


int main(int argc, char* argv[]){
    std::cout<<"Program for calculating CFD\n";
    std::cout<<"made by Mashenkin Pavel\n";

    
    {
        test_mesh_reading();

    }

    {
        testfunc1(argc,argv);
    }




    return 0;
}
