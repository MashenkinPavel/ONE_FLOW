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
    //tools for preparing solver
    size_t GetFaceCount() const{
        return faces_count;
    }
    const Face& GetFace(int iface) const{
        return mFaces[iface];
    }

    size_t GetCellCount(){
        return cells_count;

    }
private:
    int nodes_count;
    int ribs_count;
    int faces_count;
    int cells_count;
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
