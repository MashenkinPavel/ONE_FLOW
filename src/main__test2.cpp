#include <iostream>
#include <stdio.h>
#include <exception>
#include <string>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>


//include <ptscotch.h>
//#include "ptscotch.h"

#include <mpi.h>


//#if (((defined __STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)) || (defined HAVE_STDINT_H))
//#include <stdint.h>
//#endif /* (((defined __STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)) || (defined HAVE_STDINT_H)) */



#include "common.h"
#include "mesh.h"
#include "SLAE.h"


//#define ARCHNBR                     20


int test_mesh_reading();
bool prog_run(const int& argc,char* argv[]  , const std::string& mesh_name,const std::string& cfg_name);



int main(int argc, char* argv[]){
    PetscFunctionBeginUser;
    PetscInitialize( &argc, &argv, 0, 0 );

    PetscPrintf( PETSC_COMM_WORLD, "Program for calculating CFD\n" );
    PetscPrintf( PETSC_COMM_WORLD, "made by Mashenkin Pavel\n" );

    std::string mesh_name="";
    std::string cfg_name="";
    bool OK = true;
    OK = find_arg(argc,argv,"-mesh",mesh_name);
    OK = find_arg(argc,argv,"-cfg",cfg_name);


    if (false){
        //test_scotch_arch_c(argc, argv);
    }

    if (false){
        bool OK = prog_run(argc, argv, mesh_name,cfg_name);
    }


    //++++++++++++++++++++++++++++++++++++++++++++++++TESTING++++++++++++++++++++++++++++++++++++
    // mesh reading testing
    if (true)
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
    if (false){
        testfunc3(argc,argv);
    }



    PetscFinalize( );
    return 0;
}







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
    return true;
}


bool prog_run(const int& argc,char* argv[]  , const std::string& mesh_name,const std::string& cfg_name){
    Mesh mesh;
    //if (mesh.Read_SU2_file("/home/pavel/STORAGE/WORK/CFD_PROGS/ONE_FLOW/test_data/unsteady_naca0012_mesh.su2")){
    if (mesh.Read_SU2_file(mesh_name)){
        printf("SU2 file read OK!\n");
        std::string vtf_filename = mesh_name;
        vtf_filename.erase(vtf_filename.length()-3);
        vtf_filename+="vtk";
        mesh.Write_vtk_file(vtf_filename);
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
    for (int i=0;i<mesh.GetCellCount();i++)  sp_arr[i]=i;
    mesh.Write_vtk_file_and_data("debug_out.vtk",sp_arr);


    //++++++++++++++++++++++++++++++++++++++++++++
    //decomposizition mesh
    std::string val;

    if (find_arg(argc,argv,"-decomp",val));
    int nproc_decomp = std::stoi(val);








    //++++++++++++++++++++++++++++++++++++++++++++++
    Mat A;
    MatZeroEntries(A);
    MatType type=MATMPIAIJ;   //MATMPIBAIJ
    PetscInt N = mesh.GetCellCount();
    PetscInt m=2, n=3, M=8;
    MPI_Comm comm=PETSC_COMM_WORLD;
    MatCreate(comm, &A);
    MatSetSizes(A,m,n,M,N);
    MatSetType(A,type);
    MatMPIAIJSetPreallocation(A,5,PETSC_NULLPTR,5,PETSC_NULLPTR);
    MatSetUp(A);
    MatSetFromOptions(A);
    MatDestroy(&A);

    return true;
}


