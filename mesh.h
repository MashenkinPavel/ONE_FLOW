#ifndef ONE_FLOW_MESH_H
#define ONE_FLOW_MESH_H

#include "node.h"
#include "face.h"
#include "cell.h"
#include "Rib.h"

#include <vector>
#include <string>
#include <map>
class Mesh{
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
public:
    Mesh();
    ~Mesh() = default ;
    bool Read_msh_file(const std::string& filename); //reading of msh file
    bool Write_vtk_file(const std::string& filename); //writing polyhedral mesh
};

#endif