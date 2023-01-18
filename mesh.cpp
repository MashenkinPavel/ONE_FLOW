#include "mesh.h"
#include <fstream>
#include <iostream>
Mesh::Mesh() {

}

Mesh::~Mesh() {

}

bool Mesh::Read_msh_file(std::string filename) {
    return Read_msh_tetrahedrons(filename);
}

bool Mesh::Read_msh_tetrahedrons(std::string filename){
    std::ifstream fin(filename);
    if (!fin){
        std::cerr<<"File "<<filename<<" can not open!\n";
        return false;
    }
    std::cout<<"File "<<filename<<" opened, mesh is reading...\n";
    std::string buf;
    std::getline(fin,buf); //skip $MeshFormat
    std::getline(fin,buf);  //reading msh file version
    if (buf!="2.2 0 8"){
        std::cerr<<"msh file version not equal to 2.2 0 8 | Please convert msh file to 2.2 0 8\n";
        return false;
    }
    std::getline(fin,buf); //skip $EndMeshFormat
    std::getline(fin,buf); //$Nodes
    int nnodes = 0;
    fin>>nnodes;
    nodes_count = nnodes;
    for (int i=0;i<nnodes;++i){
        int temp;
        fin>>temp;
        double x,y,z;
        fin>>x>>y>>z;
        mNodeCoords.push_back(Node{x,y,z});
    }
    std::getline(fin,buf);// skip $EndNodes
    std::getline(fin,buf);// skip $Elements
    int elems=0;
    fin>>elems;
    int ncells=0;
    for (int i=0;i<ncells;++i){
        int temp;
        fin>>temp;
        fin>>temp;
        if (temp == 4){ //tetraeders
            int skip; fin>>skip;
            for (int j=0;j<skip;j++){
                fin>>temp;
            }
            int indexes[4]; //indexes of cell nodes
            for (int j=0;j<4;j++){  //4 vertices on tetraedr
                //need formulate all faces in cell
                fin>>indexes[j];
            }
            Face facearr[4];
            // обход против часовой стрелки нормаль наружу
            facearr[0].AddNode(indexes[2]);
            facearr[0].AddNode(indexes[1]);
            facearr[0].AddNode(indexes[0]);

            facearr[1].AddNode(indexes[1]);
            facearr[1].AddNode(indexes[2]);
            facearr[1].AddNode(indexes[3]);

            facearr[2].AddNode(indexes[3]);
            facearr[2].AddNode(indexes[2]);
            facearr[2].AddNode(indexes[0]);

            facearr[3].AddNode(indexes[0]);
            facearr[3].AddNode(indexes[1]);
            facearr[3].AddNode(indexes[3]);

        }
    }



    return true;
}

bool Mesh::Write_vtk_file(std::string filename) {
    return false;
}
