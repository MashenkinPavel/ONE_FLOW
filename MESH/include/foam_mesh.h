#pragma once
#include <IMesh.h>
#include <vector>
#include <array>
#include <string>
#include <map>
#include <set>
#include <Patch.h>




class foam_mesh: public IMesh{
public:
    foam_mesh() = default;
    ~foam_mesh() = default;
    bool ReadMesh(const std::string &path_to_meshfolder) override;
    void WriteMesh(const std::string &path_to_meshfolder) override;
    int GetFaceCount();
    int GetCellCount();
    void GetFace2OwnerNeibCell(int face, std::optional<int>& Owner, std::optional<int>& Neib);

    
    
    
    std::array<double,3> CellGetCenter(int icell){
        return m_cell_center[icell];
    }
    double CellGetVolume(int icell){
        return m_cell_volume[icell];
    }

    std::array<double,3> FaceGetCenter(int iface){
        return m_face_center[iface];
    }

    double FaceGetSquare(int iface){
        return m_face_square[iface];
    }
    std::array<double,3> FaceGetnorm(int iface){
        return m_face_norm[iface];
    }


    std::set<int> CellGetFaceList(int icell){
        return m_cell2faces[icell];
    }

    std::vector<int> FaceGetVertexList(int iface){
        return m_faces2vertices[iface];
    }

    std::array<double,3> VertexGetCoord(int ivert){
        return m_vertices[ivert];
    }
    
private:
    void read_vertices(std::string path_to_meshfolder);
    void read_faces(std::string path_to_meshfolder);
    void read_owner(std::string path_to_meshfolder);
    void read_neighbour(std::string path_to_meshfolder);
    void read_boundary(std::string path_to_meshfolder);
    void prepare_cell_2_faces();
    void complite_read_mesh();
    bool check_mesh();
    double CellGetVolume_private(int icell);  
    std::array<double,3> FaceGetCenter_private(int iface);      //check
    std::array<double,3> CellGetCenter_private(int icell);      //check
    double FaceGetSquare_private(int iface);                    //check
    std::array<double,3> FaceGetnorm_private(int iface);        //check

    //--------------

    //topology
    std::vector<std::array<double,3>> m_vertices;
    std::vector<std::vector<int>> m_faces2vertices;
    std::vector<int> m_owner;
    std::vector<int> m_neighbour;
    std::map<std::string, Patch> m_Patches;
    std::map<int,std::set<int>> m_cell2faces;

    //data
    std::vector< std::array<double,3> > m_cell_center;
    std::vector<double> m_cell_volume;
    std::vector<double> m_face_square;
    std::vector< std::array<double,3> > m_face_norm;
    std::vector< std::array<double,3> > m_face_center;
    
    int mCellCount = 0;



    // нумерация граней подчиняется закону:
    // [inner faces   |   bc_faces]
};