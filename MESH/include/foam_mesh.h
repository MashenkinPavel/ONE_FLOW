#pragma once
#include <IMesh.h>
#include <vector>
#include <array>
#include <string>
#include <map>
#include <set>
#include <optional>
#include <Patch.h>
#include <utility>




class foam_mesh: public IMesh{
public:
    foam_mesh() = default;
    ~foam_mesh() = default;
    bool ReadMesh(const std::string &path_to_meshfolder) override;
    void WriteMesh(const std::string &path_to_meshfolder) override;
    void GetFace2OwnerNeibCell(int face, std::optional<int>& Owner, std::optional<int>& Neib);

    
    
    
    std::array<double,3> CellGetCenter(int icell) const {
        return m_cell_center[icell];
    }
    double CellGetVolume(int icell) const {
        return m_cell_volume[icell];
    }

    std::array<double,3> FaceGetCenter(int iface) const {
        return m_face_center[iface];
    }

    double FaceGetSquare(int iface) const {
        return m_face_square[iface];
    }
    std::array<double,3> FaceGetnorm(int iface) const {
        return m_face_norm[iface];
    }


    // ===== IMesh interface =====
    int GetFaceCount() const override;
    int GetCellCount() const override;
    int GetInternalFaceCount() const override { return (int)m_neighbour.size(); }
    int GetBoundaryFaceCount() const override { return (int)m_owner.size() - (int)m_neighbour.size(); }
    int GetPatchCount() const override { return (int)m_Patches.size(); }

    std::pair<int,int> CellRange() const override { return {0, mCellCount}; }
    std::pair<int,int> InternalFaceRange() const override { return {0, (int)m_neighbour.size()}; }
    std::pair<int,int> BoundaryFaceRange() const override { return {(int)m_neighbour.size(), (int)m_owner.size()}; }
    std::pair<int,int> AllFaceRange() const override { return {0, (int)m_owner.size()}; }

    std::pair<int,int> FaceCells(int faceIdx) const override;
    std::string FaceBC(int faceIdx) const override;
    std::array<double,3> FaceCenter(int faceIdx) const override { return FaceGetCenter(faceIdx); }
    std::array<double,3> FaceNormal(int faceIdx) const override { return FaceGetnorm(faceIdx); }
    double FaceArea(int faceIdx) const override { return FaceGetSquare(faceIdx); }

    std::set<int> CellFaceList(int cellIdx) const override { return m_cell2faces.at(cellIdx); }
    std::array<double,3> CellCenter(int cellIdx) const override { return CellGetCenter(cellIdx); }
    double CellVolume(int cellIdx) const override { return CellGetVolume(cellIdx); }

    std::vector<int> FaceGetVertexList(int iface) {
        return m_faces2vertices[iface];
    }

    std::array<double,3> VertexGetCoord(int ivert) {
        return m_vertices[ivert];
    }

    std::string PatchName(int patchIdx) const override;
    std::string PatchType(int patchIdx) const override;
    int PatchFaceStart(int patchIdx) const override;
    int PatchFaceCount(int patchIdx) const override;
    
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