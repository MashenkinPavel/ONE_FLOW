#include <iostream>
#include "mesh.h"

int main(int argc, char* argv[]){
    std::cout<<"Program for calculating CFD\n";
    std::cout<<"made by Mashenkin Pavel\n";

   Mesh mesh;
   mesh.Read_msh_file("./test_data/mesh.msh");



    return 0;
}