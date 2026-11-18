#pragma once
#include "IMesh.h"
#include "node.h"
#include "face.h"
#include "cell.h"
#include "Rib.h"
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <utility>


class Mesh:public IMesh{
public:
    bool ReadMesh(const std::string &filename) override;
    void WriteMesh(const std::string &filename) override;
    Mesh();
    virtual ~Mesh(){};

    bool Read_msh_file(const std::string& filename); //reading of msh file
    bool Read_SU2_file(const std::string &filename);
    bool Write_vtk_file(const std::string& filename) const; //writing polyhedral mesh
    bool Write_vtk_file_and_data(const std::string &filename, std::shared_ptr<double[]>& data)const ;

    // ===== IMesh interface =====
    int GetFaceCount() const override { return faces_count; }
    int GetCellCount() const override { return cells_count; }
    int GetInternalFaceCount() const override { return (int)internal_face_indices_.size(); }
    int GetBoundaryFaceCount() const override { return (int)boundary_face_indices_.size(); }
    int GetPatchCount() const override { return 0; }

    std::pair<int,int> CellRange() const override { return {0, cells_count}; }
    std::pair<int,int> InternalFaceRange() const override { return {0, (int)internal_face_indices_.size()}; }
    std::pair<int,int> BoundaryFaceRange() const override { return {(int)internal_face_indices_.size(), faces_count}; }
    std::pair<int,int> AllFaceRange() const override { return {0, faces_count}; }

    std::pair<int,int> FaceCells(int faceIdx) const override;
    std::string FaceBC(int faceIdx) const override;
    std::array<double,3> FaceCenter(int faceIdx) const override;
    std::array<double,3> FaceNormal(int faceIdx) const override;
    double FaceArea(int faceIdx) const override;

    std::set<int> CellFaceList(int cellIdx) const override {
        auto it = cellToFacesList.find(cellIdx);
        if (it != cellToFacesList.end()) return it->second;
        return {};
    }
    std::array<double,3> CellCenter(int cellIdx) const override;
    double CellVolume(int cellIdx) const override;

    std::string PatchName(int) const override { return ""; }
    std::string PatchType(int) const override { return ""; }
    int PatchFaceStart(int) const override { return -1; }
    int PatchFaceCount(int) const override { return 0; }

    // ===== Legacy convenience =====
    const Face& GetFace(int iface) const { return mFaces[iface]; }
    const Cell& GetCell(int icell) const { return mCells[icell]; }
    const Node& GetNode(int inode) const { return mNodeCoords[inode]; }

private:
    int nodes_count;
    int ribs_count;
    int faces_count;
    int cells_count;

    // Pre-computed indices for fast iteration
    mutable std::vector<int> internal_face_indices_;
    mutable std::vector<int> boundary_face_indices_;

    // Cached pointers for range-based for
    mutable std::vector<const Face*> internal_face_ptrs_;
    mutable std::vector<const Face*> boundary_face_ptrs_;
    mutable std::vector<const Face*> all_face_ptrs_;

    std::vector<Node> mNodeCoords;   //list of nodes coordinates (only nodes has real coordinates)

    //Ribes based on Node indexes
    std::vector<Rib> mRibs; // (vertex1<vertex2) is a rule of any rib
    std::map<std::pair<int,int>,int> mapNdsPairNumberToRibNum;

    //Faces based on Rib indexes
    std::vector<Face> mFaces;        //list of face (index topology);  also contains node lists (like addon)

    //Cells based on Faces indexes
    std::vector<Cell> mCells;        //list of cell (index topology)
    std::map<int,std::set<int>> cellToFacesList; // int1 - номер ячейки, std::set<int> - номера граней

    bool Read_msh_tetrahedrons(const std::string& filename); //reading of tetraedrals msh file
    void prepare_all_mesh_data(); //после зачитывания сетки воспроизводит инфу по прочему соседству...
};
