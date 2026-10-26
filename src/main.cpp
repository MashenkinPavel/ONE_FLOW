#include <foam_mesh.h>
#include <memory>
#include <filesystem>
#include <iostream>




int main( int argc, char* argv[] ){
    //if(argc<2){
    //    std::cout<<"usage: "<<argv[0]<<" PATH_TO_WORKDIR"<<std::endl;
        //return 1;
    //}
    //std::filesystem::path workdir(argv[1]);
    std::shared_ptr<IMesh> sp_mesh = std::make_shared<foam_mesh>();
    std::string dir_path = "/home/pavel/STORAGE/WORK/CFD_PROGS/ONE_FLOW/MESH/tests/cell2/";
    std::string mesh_path = dir_path+"constant/polyMesh/";
    sp_mesh->ReadMesh(mesh_path);
    
    //sp_mesh->WriteMesh("WRITED_MESH");
}