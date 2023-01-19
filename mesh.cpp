#include "mesh.h"
#include <fstream>
#include <iostream>
#include <map>
#include <algorithm>
Mesh::Mesh() {
    nodes_count = 0;
    faces_count = 0;
    cells_count = 0;
}

bool Mesh::Read_msh_file(const std::string &filename) {
    return Read_msh_tetrahedrons(filename);
}

bool Mesh::Read_msh_tetrahedrons(const std::string &filename){
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
        //mNodeCoords.push_back(Node{x,y,z});
        mNodeCoords.emplace_back(Node{x,y,z});
    }
    std::getline(fin,buf);// next line
    std::getline(fin,buf);// skip $EndNodes
    std::getline(fin,buf);// skip $Elements
    int elems = 0;
    fin>>elems;
    int ncell = 0;
    std::map<std::vector<int>,int> faces_nodes_ordered;
    int face_counter = 0; // счетчик для нумерации граней
    for (int i=0;i<elems;++i){
        int temp;
        fin>>temp;
        fin>>temp;
        if (temp == 4){ //tetraeders

            int skip; fin>>skip;
            for (int j=0;j<skip;j++){
                fin>>temp;
            }
            int indexes[4]; //indexes of cell nodes
            for (auto& it:indexes){
                //need formulate all faces in cell
                fin>>it;
                it--;//indexes[j]--; //у меня нумерация с нуля узлов
            }
            Face facearr[4];
            // обход против часовой стрелки нормаль наружу
            //first face
            facearr[0].AddNode(indexes[2]);
            facearr[0].AddNode(indexes[1]);
            facearr[0].AddNode(indexes[0]);

            std::vector<int> face1{indexes[2],indexes[1],indexes[0]};
            std::sort(face1.begin(),face1.end());
            if (faces_nodes_ordered.find(face1)==faces_nodes_ordered.end()){
                faces_nodes_ordered[face1] = face_counter;
                face_counter++;
                facearr[0].SetLCell(ncell);
                mFaces.push_back(facearr[0]);
            }else{
                mFaces[faces_nodes_ordered[face1]].SetRCell(ncell);
            }

            //second face
            facearr[1].AddNode(indexes[1]);
            facearr[1].AddNode(indexes[2]);
            facearr[1].AddNode(indexes[3]);

            std::vector<int> face2{indexes[1],indexes[2],indexes[3]};
            std::sort(face2.begin(),face2.end());
            if (faces_nodes_ordered.find(face2)==faces_nodes_ordered.end()){
                faces_nodes_ordered[face2] = face_counter;
                face_counter++;
                facearr[1].SetLCell(ncell);
                mFaces.push_back(facearr[1]);
            }else{
                mFaces[faces_nodes_ordered[face2]].SetRCell(ncell);
            }

            //third face
            facearr[2].AddNode(indexes[3]);
            facearr[2].AddNode(indexes[2]);
            facearr[2].AddNode(indexes[0]);

            std::vector<int> face3{indexes[3],indexes[2],indexes[0]};
            std::sort(face3.begin(),face3.end());
            if (faces_nodes_ordered.find(face3)==faces_nodes_ordered.end()){
                faces_nodes_ordered[face3] = face_counter;
                face_counter++;
                facearr[2].SetLCell(ncell);
                mFaces.push_back(facearr[2]);
            }else{
                mFaces[faces_nodes_ordered[face3]].SetRCell(ncell);
            }

            //forth face
            facearr[3].AddNode(indexes[0]);
            facearr[3].AddNode(indexes[1]);
            facearr[3].AddNode(indexes[3]);

            std::vector<int> face4{indexes[0],indexes[1],indexes[3]};
            std::sort(face4.begin(),face4.end());
            if (faces_nodes_ordered.find(face4)==faces_nodes_ordered.end()){
                faces_nodes_ordered[face4] = face_counter;
                face_counter++;
                facearr[3].SetLCell(ncell);
                mFaces.push_back(facearr[3]);
            }else{
                mFaces[faces_nodes_ordered[face4]].SetRCell(ncell);
            }
            ncell++;
        }else{
            std::getline(fin,buf);// next line
        }

    }
    std::getline(fin,buf);// next line
    std::getline(fin,buf);// $EndElements
    cells_count = ncell;
    faces_count = face_counter;

    std::cout<<"Reading mesh comlite\n";
    std::cout<<"Nodes_count = "<<nodes_count<<std::endl;
    std::cout<<"Cells_count = "<<cells_count<<std::endl;
    std::cout<<"Face_count = "<<faces_count<<std::endl;

    return true;
}

bool Mesh::Write_vtk_file(const std::string &filename) {
    std::cout<<"Writing vtk file "<<filename<<" is started\n";
    std::ofstream out(filename);
    if (!out){
        std::cerr<<"Can not open file"<<filename<<" for writing\n";
        return false;
    }
    out<<"Mesh size = "<<cells_count<<"\n";
    out.close();
    return true;
}
