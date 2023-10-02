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

    if (false){
        try {
            Mesh mesh;
            if (mesh.Read_SU2_file("/home/pavel/STORAGE/WORK/CFD_PROGS/ONE_FLOW/test_data/cell1.su2")){
                printf("SU2 file read OK!\n");
                //mesh.Write_vtk_file("/home/pavel/STORAGE/WORK/CFD_PROGS/ONE_FLOW/test_data/unsteady_naca0012_mesh.vtk");
                mesh.Write_vtk_file("/home/pavel/STORAGE/WORK/CFD_PROGS/ONE_FLOW/test_data/cell1.vtk");
            }else{
                throw std::runtime_error("can't read SU2 mesh testfile");
            }
        }
        catch(...){
            printf("OK:    test on  problem finding file complite.\n");
        }
    }

    if (true){
        try {
            Mesh mesh;


            if (mesh.Read_SU2_file("/home/pavel/STORAGE/WORK/CFD_PROGS/ONE_FLOW/test_data/unsteady_naca0012_mesh.su2")){
                printf("SU2 file read OK!\n");
                mesh.Write_vtk_file("/home/pavel/STORAGE/WORK/CFD_PROGS/ONE_FLOW/test_data/unsteady_naca0012_mesh.vtk");
            }else{
                throw std::runtime_error("can't read SU2 mesh testfile");
            }
        }
        catch(...){
            printf("OK:    test on  problem finding file complite.\n");
        }
    }





    if (false)
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


bool prog_run(){
    Mesh mesh;
    if (mesh.Read_SU2_file("/home/pavel/STORAGE/WORK/CFD_PROGS/ONE_FLOW/test_data/unsteady_naca0012_mesh.su2")){
        printf("SU2 file read OK!\n");
        mesh.Write_vtk_file("/home/pavel/STORAGE/WORK/CFD_PROGS/ONE_FLOW/test_data/unsteady_naca0012_mesh.vtk");
    }else{
        throw std::runtime_error("can't read SU2 mesh testfile");
    }

    int boundary_faces_count = 0;
    int Nfaces = mesh.GetFaceCount();
    for (int iface = 0;iface<Nfaces;iface++){
        int lcell = mesh.GetFace(iface).GetLCell();
        int rcell = mesh.GetFace(iface).GetRCell();
        if (rcell == -1) boundary_faces_count++;
    }
    std::cout<<"Boundary_faces_count in mesh = "<<boundary_faces_count<<"\n";


    std::shared_ptr<double[]> sp_arr(new double [mesh.GetCellCount()]);// = std::make_shared<double[]>(mesh.GetCellCount());
    //sp_arr.reset(new double[mesh.GetCellCount()]);
    for (int i=0;i<mesh.GetCellCount();i++)  sp_arr[i]=i;
    mesh.Write_vtk_file_and_data("debug_out.vtk",sp_arr);


}



int main(int argc, char* argv[]){
    std::cout<<"Program for calculating CFD\n";
    std::cout<<"made by Mashenkin Pavel\n";

    //mesh reading testing
    if (false)
    {
        test_mesh_reading();
    }


    // creation of matrix testing
    if (false)
    {

        testfunc1(argc,argv);
    }
    if (false){
        testfunc2(argc,argv);
    }


    if (true){
        bool OK = prog_run();
    }


    return 0;
}
