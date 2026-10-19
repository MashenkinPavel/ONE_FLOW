#include <stdio.h>
#include <string>
#include <gtest/gtest.h>
#include <cmath>


#include <mesh.h>
#include <foam_mesh.h>


TEST(MeshTest, 1cell_openFoam_read){
    foam_mesh real_mesh;
    std::string path_to_meshfolder = "/home/pavel/STORAGE/WORK/CFD_PROGS/ONE_FLOW/MESH/tests/cell1/constant/polyMesh/";
    //printf("PATH = %s\n", path_to_meshfolder.c_str());
    bool OK = real_mesh.ReadMesh(path_to_meshfolder);
    ASSERT_TRUE(OK);
    double cellVol = real_mesh.CellGetVolume(0);
    //std::cout<<"Task = cell1 | Global_volume  = "<<cellVol<<"\n";
    EXPECT_EQ( (int) (std::fabs( cellVol - 1.0) < 1e-5), 1);
}


 TEST(MeshTest, 2cell_openFoam_read){
     foam_mesh real_mesh;
     std::string path_to_meshfolder = "/home/pavel/STORAGE/WORK/CFD_PROGS/ONE_FLOW/MESH/tests/cell2/constant/polyMesh/";
     //printf("PATH = %s\n", path_to_meshfolder.c_str());
     bool OK = real_mesh.ReadMesh(path_to_meshfolder);
     ASSERT_TRUE(OK);
     double cellVol0 = real_mesh.CellGetVolume(0);
     double cellVol1 = real_mesh.CellGetVolume(1);
     double summVoll = cellVol0 + cellVol1;
     //std::cout<<"Task = cell2 | Global_volume  = "<<summVoll<<"\n";
     EXPECT_EQ( (int) (std::fabs( summVoll - 2.0) < 1e-5), 1);
 }


TEST(MeshTest, ReadOpenFoamMesh){
    foam_mesh real_mesh;
    std::string path_to_meshfolder = "/home/pavel/STORAGE/WORK/CFD_PROGS/ONE_FLOW/MESH/tests/offsetCylinder/constant/polyMesh/";
    //printf("PATH = %s\n", path_to_meshfolder.c_str());
    bool OK = real_mesh.ReadMesh(path_to_meshfolder); 
    ASSERT_TRUE(OK);
    const int ncell = real_mesh.GetCellCount();
    double glob_vol = 0.;
    for (int icell = 0;icell<ncell;icell++){
        glob_vol+= real_mesh.CellGetVolume(icell);
    }
    
    std::cout<<"Task = offsetCylinder | Global_volume  = "<<glob_vol<<"\n";
    EXPECT_EQ( (int) (std::fabs( glob_vol - 73.7232) < 73.7232*0.0001), 1);
}