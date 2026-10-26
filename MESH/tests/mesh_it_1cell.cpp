#include <stdio.h>
#include <string>
#include <fstream>
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


TEST(MeshTest, 1cell_openFoam_write_vtu){
     foam_mesh real_mesh;
     std::string path_to_meshfolder = "/home/pavel/STORAGE/WORK/CFD_PROGS/ONE_FLOW/MESH/tests/cell1/constant/polyMesh/";
     bool OK = real_mesh.ReadMesh(path_to_meshfolder);
     ASSERT_TRUE(OK);
     
     // Write VTU - file goes to parent dir of polyMesh
     real_mesh.WriteMesh(path_to_meshfolder);
     
     // Verify VTU file exists (written to cell1/)
     std::string vtu_file = "/home/pavel/STORAGE/WORK/CFD_PROGS/ONE_FLOW/MESH/tests/cell1/cell1.vtu";
     std::ifstream f(vtu_file);
     ASSERT_TRUE(f.good());
     
     // Verify content
     std::string line;
     bool found_xml = false;
     bool found_unstructured_grid = false;
     bool found_piece = false;
     bool found_piece_cells = false;
     bool found_points = false;
     bool found_connectivity = false;
     bool found_offsets = false;
     bool found_types = false;
     bool found_cell_data = false;
     bool found_polyhedron_type = false;
     bool found_faces = false;
     bool found_faceoffsets = false;
     while (std::getline(f, line)){
         if (line.find("<?xml") != std::string::npos) found_xml = true;
         if (line.find("UnstructuredGrid") != std::string::npos) found_unstructured_grid = true;
         if (line.find("<Piece") != std::string::npos) found_piece = true;
         if (line.find("NumberOfCells=\"1\"") != std::string::npos) found_piece_cells = true;
         if (line.find("<Points>") != std::string::npos) found_points = true;
         if (line.find("Name=\"connectivity\"") != std::string::npos) found_connectivity = true;
         if (line.find("Name=\"offsets\"") != std::string::npos) found_offsets = true;
         if (line.find("Name=\"types\"") != std::string::npos) found_types = true;
         if (line.find("<CellData") != std::string::npos) found_cell_data = true;
         if (line.find("42") != std::string::npos) found_polyhedron_type = true; // VTK_POLYHEDRON
         if (line.find("Name=\"faces\"") != std::string::npos) found_faces = true;
         if (line.find("Name=\"faceoffsets\"") != std::string::npos) found_faceoffsets = true;
     }
     ASSERT_TRUE(found_xml);
     ASSERT_TRUE(found_unstructured_grid);
     ASSERT_TRUE(found_piece);
     ASSERT_TRUE(found_piece_cells);
     ASSERT_TRUE(found_points);
     ASSERT_TRUE(found_connectivity);
     ASSERT_TRUE(found_offsets);
     ASSERT_TRUE(found_types);
     ASSERT_TRUE(found_cell_data);
     ASSERT_TRUE(found_polyhedron_type);
     ASSERT_TRUE(found_faces);
     ASSERT_TRUE(found_faceoffsets);
}


TEST(MeshTest, 2cell_openFoam_write_vtu){
     foam_mesh real_mesh;
     std::string path_to_meshfolder = "/home/pavel/STORAGE/WORK/CFD_PROGS/ONE_FLOW/MESH/tests/cell2/constant/polyMesh/";
     bool OK = real_mesh.ReadMesh(path_to_meshfolder);
     ASSERT_TRUE(OK);
     
     // Write VTU - file goes to parent dir of polyMesh
     real_mesh.WriteMesh(path_to_meshfolder);
     
     // Verify VTU file exists (written to cell2/)
     std::string vtu_file = "/home/pavel/STORAGE/WORK/CFD_PROGS/ONE_FLOW/MESH/tests/cell2/cell2.vtu";
     std::ifstream f(vtu_file);
     ASSERT_TRUE(f.good());
     
     // Verify content
     std::string line;
     bool found_unstructured_grid = false;
     bool found_piece = false;
     bool found_piece_cells = false;
     bool found_piece_points = false;
     bool found_cell_data = false;
     bool found_volume = false;
     bool found_polyhedron_type = false;
     while (std::getline(f, line)){
         if (line.find("UnstructuredGrid") != std::string::npos) found_unstructured_grid = true;
         if (line.find("<Piece") != std::string::npos) found_piece = true;
         if (line.find("NumberOfCells=\"2\"") != std::string::npos) found_piece_cells = true;
         if (line.find("NumberOfPoints=\"") != std::string::npos && line.find("NumberOfCells=\"2\"") != std::string::npos) found_piece_points = true;
         if (line.find("<CellData") != std::string::npos) found_cell_data = true;
         if (line.find("cell_volume") != std::string::npos) found_volume = true;
         if (line.find("42") != std::string::npos) found_polyhedron_type = true; // VTK_POLYHEDRON
     }
     ASSERT_TRUE(found_unstructured_grid);
     ASSERT_TRUE(found_piece);
     ASSERT_TRUE(found_piece_cells);
     ASSERT_TRUE(found_piece_points);
     ASSERT_TRUE(found_cell_data);
     ASSERT_TRUE(found_volume);
     ASSERT_TRUE(found_polyhedron_type);
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


TEST(MeshTest, offsetCylinder_write_vtu){
    foam_mesh real_mesh;
    std::string path_to_meshfolder = "/home/pavel/STORAGE/WORK/CFD_PROGS/ONE_FLOW/MESH/tests/offsetCylinder/constant/polyMesh/";
    bool OK = real_mesh.ReadMesh(path_to_meshfolder); 
    ASSERT_TRUE(OK);
    
    const int ncell = real_mesh.GetCellCount();
    double glob_vol = 0.;
    for (int icell = 0;icell<ncell;icell++){
        glob_vol+= real_mesh.CellGetVolume(icell);
    }
    
    // Write VTU - file goes to parent dir of polyMesh
    real_mesh.WriteMesh(path_to_meshfolder);
    
    // Verify VTU file exists (written to offsetCylinder/)
    std::string vtu_file = "/home/pavel/STORAGE/WORK/CFD_PROGS/ONE_FLOW/MESH/tests/offsetCylinder/offsetCylinder.vtu";
    std::ifstream f(vtu_file);
    ASSERT_TRUE(f.good());
    
    // Verify content
    std::string line;
    bool found_unstructured_grid = false;
    bool found_piece = false;
    bool found_piece_cells = false;
    bool found_piece_points = false;
    bool found_cell_data = false;
    bool found_volume = false;
    bool found_connectivity = false;
    bool found_offsets = false;
    bool found_types = false;
    bool found_faces = false;
    bool found_faceoffsets = false;
    bool found_polyhedron_type = false;
    while (std::getline(f, line)){
        if (line.find("UnstructuredGrid") != std::string::npos) found_unstructured_grid = true;
        if (line.find("<Piece") != std::string::npos) found_piece = true;
        if (line.find("NumberOfCells=\"") != std::string::npos) found_piece_cells = true;
        if (line.find("NumberOfPoints=\"") != std::string::npos) found_piece_points = true;
        if (line.find("<CellData") != std::string::npos) found_cell_data = true;
        if (line.find("cell_volume") != std::string::npos) found_volume = true;
        if (line.find("Name=\"connectivity\"") != std::string::npos) found_connectivity = true;
        if (line.find("Name=\"offsets\"") != std::string::npos) found_offsets = true;
        if (line.find("Name=\"types\"") != std::string::npos) found_types = true;
        if (line.find("Name=\"faces\"") != std::string::npos) found_faces = true;
        if (line.find("Name=\"faceoffsets\"") != std::string::npos) found_faceoffsets = true;
        if (line.find("42") != std::string::npos) found_polyhedron_type = true; // VTK_POLYHEDRON
    }
    
    ASSERT_TRUE(found_unstructured_grid);
    ASSERT_TRUE(found_piece);
    ASSERT_TRUE(found_piece_cells);
    ASSERT_TRUE(found_piece_points);
    ASSERT_TRUE(found_cell_data);
    ASSERT_TRUE(found_volume);
    ASSERT_TRUE(found_connectivity);
    ASSERT_TRUE(found_offsets);
    ASSERT_TRUE(found_types);
    ASSERT_TRUE(found_faces);
    ASSERT_TRUE(found_faceoffsets);
    ASSERT_TRUE(found_polyhedron_type);
}