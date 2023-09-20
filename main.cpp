#include <iostream>
#include "mesh.h"
#include "SLAE.h"
#include <exception>

int test_mesh_reading(){
    if (false)
    {
        Mesh mesh;
        mesh.Read_msh_file("../test_data/mesh.msh");
        mesh.Write_vtk_file("../test_data/mesh.vtk");
    }


    if (false)
    {
        Mesh mesh;
        mesh.Read_msh_file("../test_data/mesh.msh");
        mesh.Write_vtk_file("../test_data/mesh.vtk");
    }


    if (false)
    {
        Mesh mesh;
        mesh.Read_msh_file("../test_data/mesh_1tetr.msh");
        mesh.Write_vtk_file("../test_data/mesh_1tetr.vtk");
    }

    if (false)
    {
        Mesh mesh;
        mesh.Read_msh_file("../test_data/mesh_2tetr.msh");
        mesh.Write_vtk_file("../test_data/mesh_2tetr.vtk");
    }


    if (false)
    {
        Mesh mesh;
        if (mesh.Read_SU2_file("../test_data/mesh_NACA0012_5deg_6814.su2")){
            printf("OK: tes SU2 file read compligte.\n");
        }else{
            throw std::runtime_error("can't read SU2 mesh testfile");
        }
    }
    
    if (false)
    {
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




    if (false){
        try {
            Mesh mesh;
            if (mesh.Read_SU2_file("/home/pavel/STORAGE/WORK/CFD_PROGS/ONE_FLOW/test_data/mesh_NACA0012_5deg_6814.su2")){
                printf("SU2 file read OK!\n");
                mesh.Write_vtk_file("/home/pavel/STORAGE/WORK/CFD_PROGS/ONE_FLOW/test_data/mesh_NACA0012_5deg_6814.vtk");

            }else{
                throw std::runtime_error("can't read SU2 mesh testfile");
            }
        }
        catch(...){
            printf("OK:    test on  problem finding file complite.\n");
        }


    }

    if (true)
    {
        try {
            Mesh mesh;
            if (mesh.Read_SU2_file("/home/pavel/STORAGE/WORK/CFD_PROGS/ONE_FLOW/test_data/square.su2")){
                printf("SU2 file read OK!\n");
                mesh.Write_vtk_file("/home/pavel/STORAGE/WORK/CFD_PROGS/ONE_FLOW/test_data/square.vtk");

            }else{
                throw std::runtime_error("can't read SU2 mesh testfile");
            }
        }
        catch(...){
            printf("OK:    test on  problem finding file complite.\n");
        }
    }
}


int main(int argc, char* argv[]){
    std::cout<<"Program for calculating CFD\n";
    std::cout<<"made by Mashenkin Pavel\n";

    if (true)
    {
        test_mesh_reading();
    }


    if (false)
    {
        testfunc1(argc,argv);

    }
    if (false){
        testfunc2(argc,argv);
    }


    return 0;
}
