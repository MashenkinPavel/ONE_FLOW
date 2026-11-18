#include <memory>
#include <filesystem>
#include <iostream>

#include <foam_mesh.h>

#include <ISolver.h>
#include <CSimple.h>

#include "general_settings.h"

const bool testmode = false;


int main( int argc, char* argv[] ){
    std::shared_ptr<IMesh> sp_mesh = std::make_shared<foam_mesh>();
    std::shared_ptr<ISolver> sp_solver;
    std::shared_ptr<Logger> sp_log; 
    if (testmode){
        std::string foam_dir = "/home/pavel/STORAGE/WORK/CFD_PROGS/ONE_FLOW/MESH/tests/offsetCylinder";
        std::string mesh_path = "/home/pavel/STORAGE/WORK/CFD_PROGS/ONE_FLOW/MESH/tests/offsetCylinder/constant/polyMesh/";
        
        std::string out_dir = foam_dir + "/ONE_FLOW_OUT";
        std::filesystem::path pth(out_dir);
        std::filesystem::create_directory(pth);

        sp_mesh->ReadMesh(mesh_path);
        sp_mesh->WriteMesh(out_dir);  
        std::shared_ptr<CGeneralSettings> sp_gsettings = std::make_shared<CGeneralSettings>();
        sp_log = std::make_shared<Logger>("log.out");
        sp_solver = std::make_shared<CSimple>(sp_mesh,sp_gsettings,sp_log);

        return 0;
    }

    if(argc<2){
        std::cout<<"usage: "<<argv[0]<<" PATH_TO_WORKDIR"<<std::endl;
        return 1;
    }
    
    std::shared_ptr<CGeneralSettings> sp_gsettings = std::make_shared<CGeneralSettings>();
    sp_gsettings->foam_dir = argv[1];
    sp_gsettings->work_dir = sp_gsettings->foam_dir + "/ONE_FLOW_OUT"; 
    std::string mesh_path = sp_gsettings->foam_dir+"/constant/polyMesh/";
    std::filesystem::path pth(sp_gsettings->work_dir);
    std::filesystem::create_directory(pth);
    std::filesystem::current_path(pth);

    sp_log = std::make_shared<Logger>("log.out");
    //sp_log << "TEST HELLO ONE FLOW\n";

    {    
        sp_mesh->ReadMesh(mesh_path);
        sp_mesh->WriteMesh(sp_gsettings->work_dir); 
        sp_log<<"PRE: mesh_path = "<< mesh_path<<"\n";
        sp_log<<"PRE: Mesh writen.\n";
    }

    if (sp_gsettings->slv_type == SolverType::SIMPLE){
        sp_solver = std::make_shared<CSimple>(sp_mesh,sp_gsettings,sp_log);
    }

    {
        sp_solver->precalc();
        sp_solver->preliminary();
        for (int tstep = 0;tstep<100;tstep++){
            sp_log<<"tstep = "<<std::to_string(tstep).c_str()<<"\n";
            sp_solver->perform_step();
            sp_solver->save_results();   
        }


    }


}