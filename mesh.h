#ifndef ONE_FLOW_MESH_H
#define ONE_FLOW_MESH_H

#include "node.h"
#include "face.h"
#include "cell.h"

#include <vector>
#include <string>
class Mesh{
    int nodes_count;
    int faces_count;
    int cells_count;
    std::vector<Node> mNodeCoords;   //list of nodes coordinates
    std::vector<Face> mFaces;        //list of face (index topology)
    std::vector<Cell> mCells;        //list of cell (index topology)
    bool Read_msh_tetrahedrons(const std::string& filename); //reading of tetraedrals msh file
public:
    Mesh();
    ~Mesh() = default ;
    bool Read_msh_file(const std::string& filename); //reading of msh file
    bool Write_vtk_file(const std::string& filename); //writing polyhedral mesh
};

#endif