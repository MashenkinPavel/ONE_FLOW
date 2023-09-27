#include "mesh.h"
#include <fstream>
#include <iostream>
#include <map>
#include <algorithm>
Mesh::Mesh() {
    nodes_count = 0;
    ribs_count = 0;
    faces_count = 0;
    cells_count = 0;
}

bool Mesh::Read_msh_file(const std::string &filename) {
    return Read_msh_tetrahedrons(filename);
}

bool Mesh::Read_msh_tetrahedrons(const std::string &filename)   {
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

    std::map<std::vector<int>,int> faces_nodes_ordered;
    int face_counter = 0; // счетчик для нумерации граней
    int rib_counter = 0;
    int ncell = 0;        //счетчик ячеек
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
            //Rib ribarr[6];

            auto elem_to_rib=[&](int v1, int v2){
                int min_ind=v1;
                int max_ind=v2;
                if (max_ind<min_ind) std::swap(max_ind,min_ind);
                std::pair<int,int> temp_rib{min_ind,max_ind};
                if (mapNdsPairNumberToRibNum.find(temp_rib) == mapNdsPairNumberToRibNum.end()){
                    //добавляется новое ребро
                    mapNdsPairNumberToRibNum[{min_ind,max_ind}] = rib_counter;
                    //mRibs.push_back(Rib(min_ind,max_ind));
                    mRibs.emplace_back(Rib(min_ind,max_ind));

                    rib_counter++;
                }else{
                    //ребро уже существует
                }
            };

            elem_to_rib(indexes[0],indexes[1]);
            elem_to_rib(indexes[1],indexes[3]);
            elem_to_rib(indexes[3],indexes[0]);
            elem_to_rib(indexes[0],indexes[2]);
            elem_to_rib(indexes[1],indexes[2]);
            elem_to_rib(indexes[3],indexes[2]);


            // для граней обход против часовой стрелки нормаль наружу
            //first face
            facearr[0].AddNode(indexes[2]);
            facearr[0].AddNode(indexes[1]);
            facearr[0].AddNode(indexes[0]);

            facearr[0].AddRib(mapNdsPairNumberToRibNum,indexes[2],indexes[1]);
            facearr[0].AddRib(mapNdsPairNumberToRibNum,indexes[1],indexes[0]);
            facearr[0].AddRib(mapNdsPairNumberToRibNum,indexes[0],indexes[2]);


            std::vector<int> face0{indexes[2],indexes[1],indexes[0]};
            std::sort(face0.begin(),face0.end());

            if (faces_nodes_ordered.find(face0)==faces_nodes_ordered.end()){
                faces_nodes_ordered[face0] = face_counter;
                cellToFacesList[ncell].insert(face_counter);
                facearr[0].SetLCell(ncell);
                mFaces.push_back(facearr[0]);
                face_counter++;
            }else{
                int face_index = faces_nodes_ordered[face0];
                mFaces[face_index].SetRCell(ncell);
                cellToFacesList[ncell].insert(face_index); //?  не точно , проверить нужно
            }

            //second face
            facearr[1].AddNode(indexes[1]);
            facearr[1].AddNode(indexes[2]);
            facearr[1].AddNode(indexes[3]);
            facearr[1].AddRib(mapNdsPairNumberToRibNum,indexes[1],indexes[2]);
            facearr[1].AddRib(mapNdsPairNumberToRibNum,indexes[2],indexes[3]);
            facearr[1].AddRib(mapNdsPairNumberToRibNum,indexes[3],indexes[1]);

            std::vector<int> face1{indexes[1],indexes[2],indexes[3]};
            std::sort(face1.begin(),face1.end());
            if (faces_nodes_ordered.find(face1)==faces_nodes_ordered.end()){
                faces_nodes_ordered[face1] = face_counter;
                cellToFacesList[ncell].insert(face_counter);
                facearr[1].SetLCell(ncell);
                mFaces.push_back(facearr[1]);
                face_counter++;
            }else{
                facearr[1].SetRCell(ncell);
                int face_index = faces_nodes_ordered[face1];
                mFaces[face_index].SetRCell(ncell);
                cellToFacesList[ncell].insert(face_index); //?  не точно , проверить нужно
            }

            //third face
            facearr[2].AddNode(indexes[3]);
            facearr[2].AddNode(indexes[2]);
            facearr[2].AddNode(indexes[0]);
            facearr[2].AddRib(mapNdsPairNumberToRibNum,indexes[3],indexes[2]);
            facearr[2].AddRib(mapNdsPairNumberToRibNum,indexes[2],indexes[0]);
            facearr[2].AddRib(mapNdsPairNumberToRibNum,indexes[0],indexes[3]);

            std::vector<int> face2{indexes[3],indexes[2],indexes[0]};
            std::sort(face2.begin(),face2.end());
            if (faces_nodes_ordered.find(face2)==faces_nodes_ordered.end()){
                faces_nodes_ordered[face2] = face_counter;
                cellToFacesList[ncell].insert(face_counter);
                facearr[2].SetLCell(ncell);
                mFaces.push_back(facearr[2]);
                face_counter++;
            }else{
                int face_index = faces_nodes_ordered[face2];
                mFaces[face_index].SetRCell(ncell);
                cellToFacesList[ncell].insert(face_index); //?  не точно , проверить нужно
            }

            //forth face
            facearr[3].AddNode(indexes[0]);
            facearr[3].AddNode(indexes[1]);
            facearr[3].AddNode(indexes[3]);
            facearr[3].AddRib(mapNdsPairNumberToRibNum,indexes[0],indexes[1]);
            facearr[3].AddRib(mapNdsPairNumberToRibNum,indexes[1],indexes[3]);
            facearr[3].AddRib(mapNdsPairNumberToRibNum,indexes[3],indexes[0]);

            std::vector<int> face3{indexes[0],indexes[1],indexes[3]};
            std::sort(face3.begin(),face3.end());
            if (faces_nodes_ordered.find(face3)==faces_nodes_ordered.end()){
                faces_nodes_ordered[face3] = face_counter;
                cellToFacesList[ncell].insert(face_counter);
                facearr[3].SetLCell(ncell);
                mFaces.push_back(facearr[3]);
                face_counter++;
            }else{
                int face_index = faces_nodes_ordered[face3];
                mFaces[face_index].SetRCell(ncell);
                cellToFacesList[ncell].insert(face_index); //?  не точно , проверить нужно
            }
            ncell++;
        }else{
            std::getline(fin,buf);// next line
        }

    }
    std::getline(fin,buf);// next line
    std::getline(fin,buf);// $EndElements
    ribs_count = rib_counter;
    cells_count = ncell;
    faces_count = face_counter;

    std::cout<<"Reading mesh comlite\n";
    std::cout<<"Nodes_count = "<<nodes_count<<std::endl;
    std::cout<<"Ribs count = "<<ribs_count<<std::endl;
    std::cout<<"Cells_count = "<<cells_count<<std::endl;
    std::cout<<"Face_count = "<<faces_count<<std::endl;

    //return true;
    prepare_all_mesh_data();
    return true;
}


bool Mesh::Read_SU2_file(const std::string &filename){
    std::string keyword;
    std::ifstream input(filename);
    if (!input) return false;
    int ndim=-1;
    input>>keyword;
    input>>ndim;
    
    if (ndim != 2){
        throw std::runtime_error("Only reading of 2d SU2 mesh corresponding\n");
    } 

    int nelem=-1;
    input>>keyword;
    input>>nelem;


    int max_node_number_topology = -1;
    //std::vector< std::vector< std::set<int> > >  mesh_2d_base(nelem); //array of cell that contain ribs array of vertices
    std::vector< std::vector< int> >  mesh_2d_base(nelem); //array of cell that contain ribs array of vertices
    int type=-1;
    for (int i=0;i<nelem;i++){
        input>>type;

        
        if (type == 5){
            //triangles
            int Node[3];
            for (int j =0;j<3;j++){
                input>>Node[j];
                if (Node[j]>max_node_number_topology)  max_node_number_topology = Node[j];
            }
            int temp=-1;
            input>>temp;
            std::vector<int> cellinfo{   Node[0],Node[1], Node[2]  };
            mesh_2d_base[i] = cellinfo;
            continue;
        }

        if (type == 9){
            //squares
            int Node[4];
            for (int j =0;j<4;j++){
                input>>Node[j];
                if (Node[j]>max_node_number_topology)  max_node_number_topology = Node[j];
            }
            int temp=-1;
            input>>temp;

            std::vector<int> cellinfo{ Node[0],Node[1],Node[2],Node[3] };
            mesh_2d_base[i] = cellinfo;
            continue;
        }
    }

    //MESH TOPOLOGY READING COMPLITE
    int npoints = -1;
    input>>keyword;
    input>>npoints;
    std::vector<   std::vector<double>   > coords;
    if ( max_node_number_topology != (npoints-1) ){
        throw std::runtime_error("number of vertices in topology not equals to geometry vertices count\n");
    }

    int locnum=-1;
    std::vector<double> R{ 0.0 ,0.0 ,0.0 };
    double min_x = 1e9, max_x = -1e9, min_y = 1e9, max_y = -1e9;

    for (int i = 0;i<(npoints);i++){
        input>>R[0];
        input>>R[1];
        input>>locnum;
        if (min_x>R[0] ) min_x = R[0];
        if (max_x<R[0] ) max_x = R[0];
        if (min_y>R[1]) min_y = R[1];
        if (max_y<R[1] )max_y = R[1];
        R[2] = 0.0;
        coords.push_back(R);
    }

    double dx_model = max_x - min_x;
    double dy_model = max_y - min_y;
    double dz_model = (dx_model>dy_model)? dx_model/10 : dy_model/10;
    

    for (int i = 0; i < (npoints) ; i++ ){
        int new_i = npoints + i;
        R[0] = coords[i][0];
        R[1] = coords[i][1];
        R[2] = dz_model;
        coords.push_back(R);
    }


    //NMARK
    int NMARKl;
   
    input>>keyword;
    input>>NMARKl;
    using bc_set = std::vector<std::vector<int>>;
    
    std::vector<  bc_set >  boundary_conditions_topo;
    std::vector<std::string> boundary_conditions_names;
 
    for (int i=0;i<NMARKl;i++){
        std::string marker_tag;
        input>>keyword;
        input>>marker_tag;
        bc_set loc_bc;
        boundary_conditions_names.push_back(marker_tag);
        int marker_elems=-1;
        input>>keyword;
        input>>marker_elems;
        for (int j=0;j<marker_elems;j++){
            int elem_number =-1;
            std::vector<int> vert_set{0,0};
            
            input>>elem_number;
            if (elem_number == 3){
                // its line
                input>>vert_set[0];
                input>>vert_set[1];
                loc_bc.push_back(vert_set);
            }
            boundary_conditions_topo.push_back(loc_bc);
        }
    }
    input.close();
//==========================================READING FILE CLOSED. PREPARING SOLVER MESH DATA....

//nodes
    nodes_count =  coords.size();
    for (int i=0;i<nodes_count;i++){
        mNodeCoords.emplace_back(Node{coords[i][0],coords[i][1],coords[i][2]});
    }
    

    std::map<std::vector<int>,int> faces_nodes_ordered;
    int face_counter = 0; // счетчик для нумерации граней
    int rib_counter = 0;
    int ncell = 0;        //счетчик ячеек

    //cells circle 
    for (int i=0;i<mesh_2d_base.size();i++){
        int nvert2d = mesh_2d_base[i].size();
        if (nvert2d == 4){
            
             

            auto elem_to_rib=[&](int v1, int v2){
                int min_ind=v1;
                int max_ind=v2;
                if (max_ind<min_ind) std::swap(max_ind,min_ind);
                std::pair<int,int> temp_rib{min_ind,max_ind};
                if (mapNdsPairNumberToRibNum.find(temp_rib) == mapNdsPairNumberToRibNum.end()){
                    //добавляется новое ребро
                    mapNdsPairNumberToRibNum[{min_ind,max_ind}] = rib_counter;
                    //mRibs.push_back(Rib(min_ind,max_ind));
                    mRibs.emplace_back(Rib(min_ind,max_ind));

                    rib_counter++;
                }else{
                    //ребро уже существует
                }
            };
            //circle on ribs on element
            
            int vert_ind[8];
            for (int j=0;j<8;j++){
                int shift = 0;
                if (j>=4) shift=npoints;

                vert_ind[ j ]  =  mesh_2d_base[i][j%4] + shift;
                //TODO check this function
            }

            //DOWN_FACE RIBS
            elem_to_rib(vert_ind[0],vert_ind[1]);
            elem_to_rib(vert_ind[1],vert_ind[2]);
            elem_to_rib(vert_ind[2],vert_ind[3]);
            elem_to_rib(vert_ind[3],vert_ind[0]);

            //UP_FACE RIBS
            elem_to_rib(vert_ind[0+4],vert_ind[1+4]);
            elem_to_rib(vert_ind[1+4],vert_ind[2+4]);
            elem_to_rib(vert_ind[2+4],vert_ind[3+4]);
            elem_to_rib(vert_ind[3+4],vert_ind[0+4]);

            //VERTICAL RIBS
            elem_to_rib(vert_ind[0],vert_ind[4]);
            elem_to_rib(vert_ind[1],vert_ind[5]);
            elem_to_rib(vert_ind[2],vert_ind[6]);
            elem_to_rib(vert_ind[3],vert_ind[7]);
            //===============================================


            // для граней обход против часовой стрелки нормаль наружу


            Face facearr[6];

            //down face
            {
                facearr[0].AddNode(vert_ind[0]);
                facearr[0].AddNode(vert_ind[3]);
                facearr[0].AddNode(vert_ind[2]);
                facearr[0].AddNode(vert_ind[1]);

                facearr[0].AddRib(mapNdsPairNumberToRibNum,vert_ind[0],vert_ind[1]);
                facearr[0].AddRib(mapNdsPairNumberToRibNum,vert_ind[1],vert_ind[2]);
                facearr[0].AddRib(mapNdsPairNumberToRibNum,vert_ind[2],vert_ind[3]);
                facearr[0].AddRib(mapNdsPairNumberToRibNum,vert_ind[3],vert_ind[0]);

                std::vector<int> face0{vert_ind[0],vert_ind[1],vert_ind[2],vert_ind[3]};
                std::sort(face0.begin(),face0.end());

                if (faces_nodes_ordered.find(face0)==faces_nodes_ordered.end()){
                    faces_nodes_ordered[face0] = face_counter;
                    cellToFacesList[ncell].insert(face_counter);
                    facearr[0].SetLCell(ncell);
                    mFaces.push_back(facearr[0]);
                    face_counter++;
                }else {
                    int face_index = faces_nodes_ordered[face0];
                    mFaces[face_index].SetRCell(ncell);
                    cellToFacesList[ncell].insert(face_index); //?  не точно , проверить нужно
                }
            }


            // TODO second face....3,4,5,6 faces
            //up face
            {
                facearr[1].AddNode(vert_ind[4]);
                facearr[1].AddNode(vert_ind[5]);
                facearr[1].AddNode(vert_ind[6]);
                facearr[1].AddNode(vert_ind[7]);

                facearr[1].AddRib(mapNdsPairNumberToRibNum,vert_ind[4],vert_ind[5]);
                facearr[1].AddRib(mapNdsPairNumberToRibNum,vert_ind[5],vert_ind[6]);
                facearr[1].AddRib(mapNdsPairNumberToRibNum,vert_ind[6],vert_ind[7]);
                facearr[1].AddRib(mapNdsPairNumberToRibNum,vert_ind[7],vert_ind[4]);

                std::vector<int> face0{vert_ind[4],vert_ind[5],vert_ind[6],vert_ind[7]};
                std::sort(face0.begin(),face0.end());

                if (faces_nodes_ordered.find(face0)==faces_nodes_ordered.end()){
                    faces_nodes_ordered[face0] = face_counter;
                    cellToFacesList[ncell].insert(face_counter);
                    facearr[1].SetLCell(ncell);
                    mFaces.push_back(facearr[1]);
                    face_counter++;
                }else {
                    int face_index = faces_nodes_ordered[face0];
                    mFaces[face_index].SetRCell(ncell);
                    cellToFacesList[ncell].insert(face_index); //?  не точно , проверить нужно
                }
            }

            //left face
            {
                facearr[2].AddNode(vert_ind[0]);
                facearr[2].AddNode(vert_ind[4]);
                facearr[2].AddNode(vert_ind[7]);
                facearr[2].AddNode(vert_ind[3]);

                facearr[2].AddRib(mapNdsPairNumberToRibNum,vert_ind[0],vert_ind[4]);
                facearr[2].AddRib(mapNdsPairNumberToRibNum,vert_ind[4],vert_ind[7]);
                facearr[2].AddRib(mapNdsPairNumberToRibNum,vert_ind[7],vert_ind[3]);
                facearr[2].AddRib(mapNdsPairNumberToRibNum,vert_ind[3],vert_ind[0]);

                std::vector<int> face0{vert_ind[0],vert_ind[4],vert_ind[7],vert_ind[3]};
                std::sort(face0.begin(),face0.end());

                if (faces_nodes_ordered.find(face0)==faces_nodes_ordered.end()){
                    faces_nodes_ordered[face0] = face_counter;
                    cellToFacesList[ncell].insert(face_counter);
                    facearr[2].SetLCell(ncell);
                    mFaces.push_back(facearr[2]);
                    face_counter++;
                }else {
                    int face_index = faces_nodes_ordered[face0];
                    mFaces[face_index].SetRCell(ncell);
                    cellToFacesList[ncell].insert(face_index); //?  не точно , проверить нужно
                }
            }

            //right face
            {
                facearr[3].AddNode(vert_ind[1]);
                facearr[3].AddNode(vert_ind[2]);
                facearr[3].AddNode(vert_ind[6]);
                facearr[3].AddNode(vert_ind[5]);

                facearr[3].AddRib(mapNdsPairNumberToRibNum,vert_ind[1],vert_ind[2]);
                facearr[3].AddRib(mapNdsPairNumberToRibNum,vert_ind[2],vert_ind[6]);
                facearr[3].AddRib(mapNdsPairNumberToRibNum,vert_ind[6],vert_ind[5]);
                facearr[3].AddRib(mapNdsPairNumberToRibNum,vert_ind[5],vert_ind[1]);

                std::vector<int> face0{vert_ind[1],vert_ind[2],vert_ind[6],vert_ind[5]};
                std::sort(face0.begin(),face0.end());

                if (faces_nodes_ordered.find(face0)==faces_nodes_ordered.end()){
                    faces_nodes_ordered[face0] = face_counter;
                    cellToFacesList[ncell].insert(face_counter);
                    facearr[3].SetLCell(ncell);
                    mFaces.push_back(facearr[3]);
                    face_counter++;
                }else {
                    int face_index = faces_nodes_ordered[face0];
                    mFaces[face_index].SetRCell(ncell);
                    cellToFacesList[ncell].insert(face_index); //?  не точно , проверить нужно
                }
            }


            //front face
            {
                facearr[4].AddNode(vert_ind[0]);
                facearr[4].AddNode(vert_ind[1]);
                facearr[4].AddNode(vert_ind[5]);
                facearr[4].AddNode(vert_ind[4]);

                facearr[4].AddRib(mapNdsPairNumberToRibNum,vert_ind[0],vert_ind[1]);
                facearr[4].AddRib(mapNdsPairNumberToRibNum,vert_ind[1],vert_ind[5]);
                facearr[4].AddRib(mapNdsPairNumberToRibNum,vert_ind[5],vert_ind[4]);
                facearr[4].AddRib(mapNdsPairNumberToRibNum,vert_ind[4],vert_ind[0]);

                std::vector<int> face0{vert_ind[0],vert_ind[1],vert_ind[5],vert_ind[4]};
                std::sort(face0.begin(),face0.end());

                if (faces_nodes_ordered.find(face0)==faces_nodes_ordered.end()){
                    faces_nodes_ordered[face0] = face_counter;
                    cellToFacesList[ncell].insert(face_counter);
                    facearr[4].SetLCell(ncell);
                    mFaces.push_back(facearr[4]);
                    face_counter++;
                }else {
                    int face_index = faces_nodes_ordered[face0];
                    mFaces[face_index].SetRCell(ncell);
                    cellToFacesList[ncell].insert(face_index); //?  не точно , проверить нужно
                }
            }

            //back face
            {
                facearr[5].AddNode(vert_ind[3]);
                facearr[5].AddNode(vert_ind[7]);
                facearr[5].AddNode(vert_ind[6]);
                facearr[5].AddNode(vert_ind[2]);

                facearr[5].AddRib(mapNdsPairNumberToRibNum,vert_ind[3],vert_ind[7]);
                facearr[5].AddRib(mapNdsPairNumberToRibNum,vert_ind[7],vert_ind[6]);
                facearr[5].AddRib(mapNdsPairNumberToRibNum,vert_ind[6],vert_ind[2]);
                facearr[5].AddRib(mapNdsPairNumberToRibNum,vert_ind[2],vert_ind[3]);

                std::vector<int> face0{vert_ind[3],vert_ind[7],vert_ind[6],vert_ind[2]};
                std::sort(face0.begin(),face0.end());

                if (faces_nodes_ordered.find(face0)==faces_nodes_ordered.end()){
                    faces_nodes_ordered[face0] = face_counter;
                    cellToFacesList[ncell].insert(face_counter);
                    facearr[5].SetLCell(ncell);
                    mFaces.push_back(facearr[5]);
                    face_counter++;
                }else {
                    int face_index = faces_nodes_ordered[face0];
                    mFaces[face_index].SetRCell(ncell);
                    cellToFacesList[ncell].insert(face_index); //?  не точно , проверить нужно
                }
            }
            ncell++;
            continue;
        }//if (nvert2d == 4)


        //====================================================================================
        //triangle element
        if (nvert2d == 3){ // if  triangles
            auto elem_to_rib=[&](int v1, int v2){
                int min_ind=v1;
                int max_ind=v2;
                if (max_ind<min_ind) std::swap(max_ind,min_ind);
                std::pair<int,int> temp_rib{min_ind,max_ind};
                if (mapNdsPairNumberToRibNum.find(temp_rib) == mapNdsPairNumberToRibNum.end()){
                    //добавляется новое ребро
                    mapNdsPairNumberToRibNum[{min_ind,max_ind}] = rib_counter;
                    //mRibs.push_back(Rib(min_ind,max_ind));
                    mRibs.emplace_back(Rib(min_ind,max_ind));

                    rib_counter++;
                }else{
                    //ребро уже существует
                }
            };
            

            int vert_ind[6];
            for (int j=0;j<6;j++){
                int shift = 0;
                if (j>=3) shift=npoints;

                vert_ind[ j ]  =  mesh_2d_base[i][j%3] + shift;
                //TODO check this function
            }

            //DOWN_FACE
            elem_to_rib(vert_ind[0],vert_ind[1]);
            elem_to_rib(vert_ind[1],vert_ind[2]);
            elem_to_rib(vert_ind[2],vert_ind[0]);


            //UP_FACE
            elem_to_rib(vert_ind[0+3],vert_ind[1+3]);
            elem_to_rib(vert_ind[1+3],vert_ind[2+3]);
            elem_to_rib(vert_ind[2+3],vert_ind[3]);

            
            elem_to_rib(vert_ind[0],vert_ind[3]);
            elem_to_rib(vert_ind[1],vert_ind[4]);
            elem_to_rib(vert_ind[2],vert_ind[5]);
            //===============================================


            // для граней обход против часовой стрелки нормаль наружу
            Face facearr[5];
            //first face
            {
                
                facearr[0].AddNode(vert_ind[0]);
                facearr[0].AddNode(vert_ind[2]);
                facearr[0].AddNode(vert_ind[1]);

                facearr[0].AddRib(mapNdsPairNumberToRibNum,vert_ind[0],vert_ind[2]);
                facearr[0].AddRib(mapNdsPairNumberToRibNum,vert_ind[2],vert_ind[1]);
                facearr[0].AddRib(mapNdsPairNumberToRibNum,vert_ind[1],vert_ind[0]);
   

                std::vector<int> face0{vert_ind[0],vert_ind[2],vert_ind[1]};
                std::sort(face0.begin(),face0.end());

                if (faces_nodes_ordered.find(face0)==faces_nodes_ordered.end()){
                    faces_nodes_ordered[face0] = face_counter;
                    cellToFacesList[ncell].insert(face_counter);
                    facearr[0].SetLCell(ncell);
                    mFaces.push_back(facearr[0]);
                    face_counter++;
                }else{
                    int face_index = faces_nodes_ordered[face0];
                    mFaces[face_index].SetRCell(ncell);
                    cellToFacesList[ncell].insert(face_index); //?  не точно , проверить нужно
                }
            }

            //second face
            {

                facearr[1].AddNode(vert_ind[3]);
                facearr[1].AddNode(vert_ind[4]);
                facearr[1].AddNode(vert_ind[5]);


                facearr[1].AddRib(mapNdsPairNumberToRibNum,vert_ind[3],vert_ind[4]);
                facearr[1].AddRib(mapNdsPairNumberToRibNum,vert_ind[4],vert_ind[5]);
                facearr[1].AddRib(mapNdsPairNumberToRibNum,vert_ind[5],vert_ind[3]);
                

                std::vector<int> face1{vert_ind[3],vert_ind[4],vert_ind[5]};
                std::sort(face1.begin(),face1.end());

                if (faces_nodes_ordered.find(face1)==faces_nodes_ordered.end()){
                    faces_nodes_ordered[face1] = face_counter;
                    cellToFacesList[ncell].insert(face_counter);
                    facearr[1].SetLCell(ncell);
                    mFaces.push_back(facearr[1]);
                    face_counter++;
                }else{
                    int face_index = faces_nodes_ordered[face1];
                    mFaces[face_index].SetRCell(ncell);
                    cellToFacesList[ncell].insert(face_index); //?  не точно , проверить нужно
                }
            }


            {
                //third face
                facearr[2].AddNode(vert_ind[0]);
                facearr[2].AddNode(vert_ind[1]);
                facearr[2].AddNode(vert_ind[4]);
                facearr[2].AddNode(vert_ind[3]);

                facearr[2].AddRib(mapNdsPairNumberToRibNum,vert_ind[0],vert_ind[1]);
                facearr[2].AddRib(mapNdsPairNumberToRibNum,vert_ind[1],vert_ind[4]);
                facearr[2].AddRib(mapNdsPairNumberToRibNum,vert_ind[4],vert_ind[3]);
                facearr[2].AddRib(mapNdsPairNumberToRibNum,vert_ind[3],vert_ind[0]);

                std::vector<int> face2{vert_ind[0],vert_ind[1],vert_ind[4], vert_ind[3]};

                std::sort(face2.begin(),face2.end());
                if (faces_nodes_ordered.find(face2)==faces_nodes_ordered.end()){
                    faces_nodes_ordered[face2] = face_counter;
                    cellToFacesList[ncell].insert(face_counter);
                    facearr[2].SetLCell(ncell);
                    mFaces.push_back(facearr[2]);
                    face_counter++;
                }else{
                    int face_index = faces_nodes_ordered[face2];
                    mFaces[face_index].SetRCell(ncell);
                    cellToFacesList[ncell].insert(face_index); //?  не точно , проверить нужно
                }
            }



            {
                //4face
                facearr[3].AddNode(vert_ind[1]);
                facearr[3].AddNode(vert_ind[2]);
                facearr[3].AddNode(vert_ind[5]);
                facearr[3].AddNode(vert_ind[4]);

                facearr[3].AddRib(mapNdsPairNumberToRibNum,vert_ind[1],vert_ind[2]);
                facearr[3].AddRib(mapNdsPairNumberToRibNum,vert_ind[2],vert_ind[5]);
                facearr[3].AddRib(mapNdsPairNumberToRibNum,vert_ind[5],vert_ind[4]);
                facearr[3].AddRib(mapNdsPairNumberToRibNum,vert_ind[4],vert_ind[1]);

                std::vector<int> face3{vert_ind[1],vert_ind[2],vert_ind[5], vert_ind[4]};

                std::sort(face3.begin(),face3.end());
                if (faces_nodes_ordered.find(face3)==faces_nodes_ordered.end()){
                    faces_nodes_ordered[face3] = face_counter;
                    cellToFacesList[ncell].insert(face_counter);
                    facearr[3].SetLCell(ncell);
                    mFaces.push_back(facearr[3]);
                    face_counter++;
                }else{
                    int face_index = faces_nodes_ordered[face3];
                    mFaces[face_index].SetRCell(ncell);
                    cellToFacesList[ncell].insert(face_index); //?  не точно , проверить нужно
                }
            }

            {
                //5face
                facearr[4].AddNode(vert_ind[0]);
                facearr[4].AddNode(vert_ind[3]);
                facearr[4].AddNode(vert_ind[5]);
                facearr[4].AddNode(vert_ind[2]);

                facearr[4].AddRib(mapNdsPairNumberToRibNum,vert_ind[0],vert_ind[3]);
                facearr[4].AddRib(mapNdsPairNumberToRibNum,vert_ind[3],vert_ind[5]);
                facearr[4].AddRib(mapNdsPairNumberToRibNum,vert_ind[5],vert_ind[2]);
                facearr[4].AddRib(mapNdsPairNumberToRibNum,vert_ind[2],vert_ind[0]);

                std::vector<int> face4{vert_ind[0],vert_ind[3],vert_ind[5], vert_ind[2]};

                std::sort(face4.begin(),face4.end());
                if (faces_nodes_ordered.find(face4)==faces_nodes_ordered.end()){
                    faces_nodes_ordered[face4] = face_counter;
                    cellToFacesList[ncell].insert(face_counter);
                    facearr[4].SetLCell(ncell);
                    mFaces.push_back(facearr[4]);
                    face_counter++;
                }else{
                    int face_index = faces_nodes_ordered[face4];
                    mFaces[face_index].SetRCell(ncell);
                    cellToFacesList[ncell].insert(face_index); //?  не точно , проверить нужно
                }
            }
            ncell++;
            continue;
        }//if (nvert2d == 3)
    }//for (int i=0;i<mesh_2d_base.size();i++)

    ribs_count = rib_counter;
    cells_count = ncell;
    faces_count = face_counter;


    std::cout<<"Reading mesh comlite\n";
    std::cout<<"Nodes_count = "<<nodes_count<<std::endl;
    std::cout<<"Ribs count = "<<ribs_count<<std::endl;
    std::cout<<"Cells_count = "<<cells_count<<std::endl;
    std::cout<<"Face_count = "<<faces_count<<std::endl;

    //return true;
    prepare_all_mesh_data();

    return true;
}
//запись полиэдральной сетки в формате VTK
bool Mesh::Write_vtk_file(const std::string &filename) const {
    std::cout<<"Writing vtk file "<<filename<<" is started\n";
    std::ofstream out(filename);
    if (!out){
        std::cerr<<"Can not open file"<<filename<<" for writing\n";
        return false;
    }
    //out<<"Mesh size = "<<cells_count<<"\n";
    out<<"# vtk DataFile Version 2.0"<<"\n";
    out<<"Unstructured Grid Example"<<"\n";
    out<<"ASCII"<<"\n\n";
    out<<"DATASET UNSTRUCTURED_GRID\n";
    out<<"POINTS "<<nodes_count<<" float\n";
    for (int i=0;i<nodes_count;++i){
        out<<mNodeCoords.at(i).GetX()<<" "<<\
             mNodeCoords.at(i).GetY()<<" "<<\
             mNodeCoords.at(i).GetZ()<<" "<<"\n";
    }


    const bool triangles = false;
    if (triangles){
        out<<"CELLS "<<cells_count<<" "<<25*cells_count<<"\n";
        for (int i=0;i<cells_count;i++){
            out<<24<<" "<<5<<"\n";
            const std::vector<int>& facelst = mCells.at(i).CellGetFaceList();//mCells[i].CellGetFaceList();
            for (const auto& it:facelst){
                out<<mFaces.at(it).GetNodeList().size();
                 for (auto nodes:mFaces.at(it).GetNodeList()){
                     out<<" "<<nodes;
                 }
                out<<"\n";
            }
        }
    }

    const bool hex = true;
    if (hex){
        out<<"CELLS "<<cells_count<<" "<<31*cells_count + cells_count<<"\n";
        for (int i=0;i<cells_count;i++){
            out<<31<<" "<<6<<"\n";

            const std::vector<int>& facelst = mCells.at(i).CellGetFaceList();//mCells[i].CellGetFaceList();

            for (const auto& it:facelst){
                out<<mFaces.at(it).GetNodeList().size();
                auto tempvar = mFaces.at(it).GetNodeList();
                for (auto nodes:mFaces.at(it).GetNodeList()){
                    out<<" "<<nodes;
                }
                out<<"\n";
            }
        }
    }





    out<<"CELL_TYPES "<<cells_count<<"\n";
    for (int i=0;i<cells_count;i++) out<<42<<"\n";
    out.close();
    std::cout<<"Mesh writing is complite!\n";
    return true;
}




void Mesh::prepare_all_mesh_data() {
    int cell_count = cellToFacesList.size();
    mCells.resize(cell_count);
    for (auto item:cellToFacesList){
        int cellnum = item.first;
        std::set<int> facelist = item.second;
        std::vector<int> vecfacelist(facelist.size());
        int i=0;
        for (auto it:facelist){
            vecfacelist[i] = it;
            i++;
        }
        Cell objcell;
        objcell.CellSetFaceList(vecfacelist);
        mCells[cellnum] = objcell;
    }
}
