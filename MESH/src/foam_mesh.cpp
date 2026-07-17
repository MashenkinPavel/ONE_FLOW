#include <cstdlib>
#include <fstream>
#include <exception>
#include <filesystem>
#include <iostream>
#include <regex>
#include <assert.h>
#include <set>

#include <foam_mesh.h>
#include <vector_operations.h>

    bool foam_mesh::ReadMesh(const std::string &path_to_meshfolder) {
        read_vertices(path_to_meshfolder);
        read_faces(path_to_meshfolder);
        read_owner(path_to_meshfolder);
        read_neighbour(path_to_meshfolder);
        read_boundary(path_to_meshfolder);
        prepare_cell_2_faces();
        complite_read_mesh();
        bool OK = check_mesh();
        return OK;
    }
    
    void foam_mesh::WriteMesh(const std::string &filename) {
        
    }
    
    void foam_mesh::read_vertices(std::string path_to_meshfolder){
        std::string path_to_file_vertices;
        for (const auto& entry : std::filesystem::directory_iterator(path_to_meshfolder)) {
            //std::cout << entry.path().filename()<<std::endl;
            if (entry.path().filename() == "points"){
                path_to_file_vertices = path_to_meshfolder/entry.path().filename();
            }
        }
        std::fstream file(path_to_file_vertices);
        if (!file) throw std::runtime_error("cannot open file");
        //отключение синхронизации с stdio для ускорения
        std::ios_base::sync_with_stdio(false);
        file.tie(nullptr);
        std::string content;
        std::string line;
        
    


        while (std::getline(file, line)) {
            if (line == "// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //") break;
        }
        while (std::getline(file, line)){
            if (!content.empty()) content += " ";
            content += line;
        }

        std::regex numRegex(R"([-+]?\d*\.?\d+(?:[eE][-+]?\d+)?)");
        std::sregex_iterator it(content.begin(), content.end(), numRegex);
        std::sregex_iterator end;
        std::vector<double> values;
        for (; it != end; ++it) {
            values.push_back(std::stod(it->str()));
        }
        int vert_count = values[0];
        for (int i=0;i<vert_count; i++){
            double x = values[i*3 + 1];
            double y = values[i*3 + 2];
            double z = values[i*3 + 3];

            std::array<double,3> vec3{x,y,z};  
            m_vertices.push_back(vec3);  
        }
        printf("FILE = %s     Nvertices = %ld\n", path_to_file_vertices.c_str() ,m_vertices.size());
    }


    void foam_mesh::read_faces(std::string path_to_meshfolder){
        std::string path_to_file_faces;
        for (const auto& entry : std::filesystem::directory_iterator(path_to_meshfolder)) {
            //std::cout << entry.path().filename()<<std::endl;
            if (entry.path().filename() == "faces"){
                path_to_file_faces = path_to_meshfolder/entry.path().filename();
            }
        }
        std::fstream file(path_to_file_faces);
        if (!file) throw std::runtime_error("cannot open file");
        //отключение синхронизации с stdio для ускорения
        std::ios_base::sync_with_stdio(false);
        file.tie(nullptr);
        std::string line;

    
        while (std::getline(file, line)) {
            if (line == "(") break;
        }

        std::regex numPattern(R"(-?\d+)");
        std::string str;

        while (std::getline(file, str, '\0'))  // читаем до конца (char(0))
        {
            
            //получить строку из файла
            auto begin = std::sregex_iterator(str.begin(), str.end(), numPattern);
            auto end = std::sregex_iterator();
            
            for (auto it = begin; it != end; ++it) {  
                int n_vertices_on_face =std::stoi(it->str());
                std::vector<int> face2vert;
                for (int i=0;i<n_vertices_on_face;i++){
                    ++it;
                    face2vert.push_back(std::stoi(it->str()));
                }
                m_faces2vertices.push_back(face2vert);
            }
            
        }
        printf("FILE = %s     Nfaces = %ld\n", path_to_file_faces.c_str() ,m_faces2vertices.size());
    }

    void foam_mesh::read_owner(std::string path_to_meshfolder){
        std::string path_to_file_owner;
        for (const auto& entry : std::filesystem::directory_iterator(path_to_meshfolder)) {
            //std::cout << entry.path().filename()<<std::endl;
            if (entry.path().filename() == "owner"){
                path_to_file_owner = path_to_meshfolder/entry.path().filename();
            }
        }
        std::fstream file(path_to_file_owner);
        if (!file) throw std::runtime_error("cannot open file");
        //отключение синхронизации с stdio для ускорения
        std::ios_base::sync_with_stdio(false);
        file.tie(nullptr);
        std::string content;
        std::string line;
        while (std::getline(file, line)) {
            if (line == "// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //") break;
        }

        //6{0}
        while (std::getline(file, line)){
            if (!content.empty()) content += " ";
            content += line;
        }

        {
            std::regex init_list(R"((\d+)\{(\d+)\})");
            std::smatch m;
            if (std::regex_search(content, m, init_list)) {
                int sz = std::stoi(m[1]);
                int val = std::stoi(m[2]);
                m_owner.resize(sz,val);
                std::set<int> celllist;
                for (int icell:m_owner){
                    celllist.insert( icell );
                }
                const int Ncell =  celllist.size();
                mCellCount = Ncell;
                return;
            }
        }
        
        {
            std::regex numRegex(R"([-+]?\d*\.?\d+(?:[eE][-+]?\d+)?)");
            std::sregex_iterator it(content.begin(), content.end(), numRegex);
            std::sregex_iterator end;
            std::vector<int> values;
            for (; it != end; ++it) {
                values.push_back(std::stoi(it->str()));
            }
            const int face_count = values[0];
            m_owner.resize(face_count);
            std::set<int> celllist;
            for (int i=0;i< face_count;i++){
                m_owner[i] = values[i+1];
                celllist.insert( m_owner[i] );
            }
            const int Ncell =  celllist.size();
            mCellCount = Ncell;
        }
    
        printf("FILE = %s     N_owner = %ld\n", path_to_file_owner.c_str() ,m_owner.size());
    }

    void foam_mesh::read_neighbour(std::string path_to_meshfolder){
        std::string path_to_file_neighbour;
        for (const auto& entry : std::filesystem::directory_iterator(path_to_meshfolder)) {
            //std::cout << entry.path().filename()<<std::endl;
            if (entry.path().filename() == "neighbour"){
                path_to_file_neighbour = path_to_meshfolder/entry.path().filename();
            }
        }
        std::fstream file(path_to_file_neighbour);
        if (!file) throw std::runtime_error("cannot open file");
        //отключение синхронизации с stdio для ускорения
        std::ios_base::sync_with_stdio(false);
        file.tie(nullptr);

        std::string content;
        std::string line;
        while (std::getline(file, line)) {
            if (line == "// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //") break;
        }

        while (std::getline(file, line)){
            if (!content.empty()) content += " ";
            content += line;
        }
        
        std::regex numRegex(R"([-+]?\d*\.?\d+(?:[eE][-+]?\d+)?)");
        auto begin = std::sregex_iterator(content.begin(), content.end(), numRegex);
        auto end = std::sregex_iterator();

        std::vector<double> numbers;
        for (auto it = begin; it != end; ++it) {
            numbers.push_back(std::stod(it->str()));
        }
        
        int count = numbers[0];
        m_neighbour.resize(count);
        for (int i=0;i<count;i++){
            m_neighbour[i] = numbers[i+1];
        }
        printf("FILE = %s     Nneighbour = %ld\n", path_to_file_neighbour.c_str() ,m_neighbour.size());
    }

    void foam_mesh::read_boundary(std::string path_to_meshfolder){
        std::string path_to_file_boundary;
        for (const auto& entry : std::filesystem::directory_iterator(path_to_meshfolder)) {
            //std::cout << entry.path().filename()<<std::endl;
            if (entry.path().filename() == "boundary"){
                path_to_file_boundary = path_to_meshfolder/entry.path().filename();
            }
        }
        std::fstream file(path_to_file_boundary);
        if (!file) throw std::runtime_error("cannot open file");
        //отключение синхронизации с stdio для ускорения
        std::ios_base::sync_with_stdio(false);
        file.tie(nullptr);

        std::string content((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());
        file.close();
        
        
        std::regex pattern(R"((\d+)\s*\n\s*\()");
        std::smatch match;

        if (!std::regex_search(content, match, pattern)) {
            assert(0);    
        }      
        int n_patches = std::stoi(match[1].str());


        std::stringstream content_stream(content);
        
        while (content_stream){
            std::string temp;
            content_stream>>temp;
            if (temp == "(") break;
        }
        
        for (int patch_index = 0;patch_index< n_patches;patch_index++){
            std::string patch_name;
            content_stream>>patch_name;
            Patch localpatch;
            fprintf(stdout,"----------------- patchname = %s ----------------------\n",patch_name.c_str());
            localpatch.set_name(patch_name);
            char ch;
            content_stream >> ch;
            if (ch=='{'){
                while (content_stream >> ch && ch!='}'){
                    content_stream.putback(ch);
                    {//read params
                        std::string param_name;
                        content_stream >> param_name;
                        if (param_name == "type"){
                            std::string attribute_name;
                            content_stream>>attribute_name;
                            attribute_name.erase(attribute_name.end()-1);
                            fprintf(stdout, "param_name = %s  with attribute = %s\n",param_name.c_str(), attribute_name.c_str());
                            
                            localpatch.set_type(attribute_name);

                        }else if (param_name == "nFaces"){
                            std::string attribute_name;
                            content_stream>>attribute_name;
                            attribute_name.erase(attribute_name.end()-1);
                            int nFaces = std::stoi(attribute_name);
                            fprintf(stdout, "param_name = %s  with attribute = %s\n",param_name.c_str(), attribute_name.c_str());
                            localpatch.set_nFaces(nFaces);
                        }else if (param_name == "startFace"){
                            std::string attribute_name;
                            content_stream>>attribute_name;
                            attribute_name.erase(attribute_name.end()-1);
                            int startFace = std::stoi(attribute_name);
                            fprintf(stdout, "param_name = %s  with attribute = %s\n",param_name.c_str(), attribute_name.c_str());
                            localpatch.set_startFace(startFace);
                        }else {
                            //std::string attribute_name;
                            char buf[1024];
                            content_stream.getline(buf,1024);
                            //content_stream>>attribute_name;
                            fprintf(stderr, "unknown param = %s   with attribute = %s\n", param_name.c_str(), buf);
                        }
                    }
                }
            }
            m_Patches[localpatch.get_name()] = localpatch;
        }
    }

    void foam_mesh::GetFace2OwnerNeibCell(int face_index, std::optional<int>& Owner, std::optional<int>& Neib){
        *Owner = m_owner[face_index];
        if (face_index<m_neighbour.size()){
            *Neib = m_neighbour[face_index];
        } 
        else Neib.reset();
    }

    int foam_mesh::GetFaceCount(){
        return m_owner.size();
    }

    int foam_mesh::GetCellCount(){
        return mCellCount;
        //m_owner.size();
    }

    void foam_mesh::prepare_cell_2_faces(){
        int inner_face_count = m_neighbour.size();
        // std::map<int, int> cell_2_face;
        
        for (int innerface = 0; innerface<inner_face_count;innerface++){
            int face_index = innerface;
            int owner = m_owner[face_index];
            int neighbour = m_neighbour[face_index];
            m_cell2faces[owner].insert(face_index) ;
            m_cell2faces[neighbour].insert(face_index) ;
        }

        int boundary_face_count = m_owner.size() -  m_neighbour.size();
        for (int bc_face = 0; bc_face< boundary_face_count; bc_face++){
            int face_index = inner_face_count + bc_face;
            int owner = m_owner[face_index];
            int neighbour = -1;
            m_cell2faces[owner].insert(face_index);
        }

    }

    std::array<double,3> foam_mesh::FaceGetCenter_private(int iface){
        std::array<double,3> result{0.0};
        const std::vector<int>& vertices = m_faces2vertices[iface];
        assert(vertices.size()>=3);
        std::vector< std::array<double,3> > vertices_coords( vertices.size(), std::array<double,3>() );  
        for(int i=0;i<vertices.size();i++){
            vertices_coords[i][0] = m_vertices[ vertices[i] ][0];
            vertices_coords[i][1] = m_vertices[ vertices[i] ][1];
            vertices_coords[i][2] = m_vertices[ vertices[i] ][2];
        }

        if (vertices.size() == 3){
            const std::array<double,3>& coord0  = vertices_coords[0];
            const std::array<double,3>& coord1  = vertices_coords[1];
            const std::array<double,3>& coord2  = vertices_coords[2];
            result[0] += (1./3.)*(coord0[0] +coord1[0] + coord2[0] );
            result[1] += (1./3.)*(coord0[1] +coord1[1] + coord2[1] );
            result[2] += (1./3.)*(coord0[2] +coord1[2] + coord2[2] );
        }else{
            const double ves = 1./vertices_coords.size();
            for (const auto& coords:vertices_coords){
                result[0] += (ves)*coords[0];
                result[1] += (ves)*coords[1];
                result[2] += (ves)*coords[2];
            }
        }
        return result;
    }

    double foam_mesh::FaceGetSquare_private(int iface){
        //  watch in openfoam  void Foam::primitiveMesh::makeFaceCentresAndAreas
        double result = 0.0;
        const std::vector<int>& vertices = m_faces2vertices[iface];
        if (vertices.size() == 3){
            std::array<double,3> vec1  = m_vertices[ vertices[1]  ] - m_vertices[vertices[0]];
            std::array<double,3> vec2  = m_vertices[ vertices[2] ] - m_vertices[ vertices[1] ];
            std::array<double,3> Sp = vector_operations::cross_product(vec1,vec2);
            double S_tr = 0.5 * vector_operations::length(Sp);
            result = S_tr;
        }
        else {
            std::array<double,3> centrXYZ = FaceGetCenter(iface);
            for (int i =0;i < vertices.size();i++){
                std::array<double,3> vec1;
                std::array<double,3> vec2;
                if (i < (vertices.size()-1)) {
                    vec1  = m_vertices[  vertices[i+1] ] - m_vertices[ vertices[i] ];
                    vec2  = centrXYZ - m_vertices[ vertices[i+1] ];
                }else{
                    vec1  = m_vertices[ vertices[0] ] - m_vertices[ vertices[i] ];
                    vec2  = centrXYZ - m_vertices[ vertices[0] ];
                }
                std::array<double,3> Sp = vector_operations::cross_product(vec1,vec2);
                double S_tr = 0.5 * vector_operations::length(Sp);
                result += S_tr;
            }
        }        
        return result;
    }

    std::array<double,3> foam_mesh::FaceGetnorm_private(int iface){
        std::array<double,3> result{0.0};
        std::array<double,3> centrXYZ = FaceGetCenter(iface);
        const std::vector<int>& vertices = m_faces2vertices[iface];
        const double face_square = FaceGetSquare(iface);
        for (int i =0;i < vertices.size();i++){
                std::array<double,3> vec1;
                std::array<double,3> vec2;
                if ((i+1) == vertices.size()){
                    vec1  = m_vertices[  vertices[0] ] - m_vertices[ vertices[i] ];
                    vec2  = centrXYZ - m_vertices[vertices[0]];
                }
                else{
                    vec1  = m_vertices[  vertices[i+1] ] - m_vertices[ vertices[i] ];
                    vec2  = centrXYZ - m_vertices[vertices[i+1]];
                }
                std::array<double,3> Sp = vector_operations::cross_product(vec1,vec2)*0.5;
                double Sp_mag = vector_operations::length(Sp);
                std::array<double,3> localface_norm = Sp*(1./Sp_mag)*(Sp_mag/face_square);
                result = result + localface_norm;
        }
        double res_length = vector_operations::length(result);  
        result = result * (1./res_length);
        return result;
    }
    
    double foam_mesh::CellGetVolume_private(int icell){
        //void Foam::primitiveMesh::calcCellCentresAndVols()
        
        //volume of the cell is pyramid volume
        
        double result = 0.;
        std::array<double,3> cellcentr =  CellGetCenter(icell);
        
        for (const auto& face_ind : m_cell2faces.at(icell) )
        {    
            std::array<double,3> face_centr = FaceGetCenter(face_ind);
            const std::vector<int>& vertices = m_faces2vertices[face_ind];
            double add_volume = 0.;
            for (int i = 0;i < vertices.size();i++){
                std::array<double,3> vert0 = m_vertices[ vertices[i] ];
                std::array<double,3> vert1;
                if ((i+1)==vertices.size()){
                    vert1 = m_vertices[vertices[0]];
                }else{
                    vert1 = m_vertices[ vertices[i+1] ];
                }
                std::array<double,3> vecA = vert0 - cellcentr;
                std::array<double,3> vecB = vert1 - cellcentr;
                std::array<double,3> vecC = face_centr - cellcentr;
                double temp_volume = vector_operations::TetrVolume(vecA,vecB,vecC );
                add_volume += temp_volume;
                result += temp_volume;
            }

            //for debuging
            //std::cout<<"cell = <<"<<icell<<"   adfacevol = "<<face_ind<<" / "<< add_volume<<std::endl;
        }
        

        
        
        
        return result;
    }


    std::array<double,3> foam_mesh::CellGetCenter_private(int icell){
        const int face_count = m_cell2faces[icell].size();
        double weight = 1./face_count;
        std::array<double,3> cell_centr{0.};
        for (const auto& face_ind:m_cell2faces[icell]){
            cell_centr = cell_centr + FaceGetCenter(face_ind)*weight;
        }
        return cell_centr;
    }



    void foam_mesh::complite_read_mesh(){

        const int NFace_count = GetFaceCount();
        m_face_center.resize(NFace_count);
        m_face_square.resize(NFace_count);
        m_face_norm.resize(NFace_count);


        for (int iface=0;iface<NFace_count;iface++){
            m_face_center[iface] = FaceGetCenter_private(iface);
        }
        for (int iface=0;iface<NFace_count;iface++){
            m_face_square[iface] = FaceGetSquare_private(iface);
        }
        for (int iface=0;iface<NFace_count;iface++){
            m_face_norm[iface] =  FaceGetnorm_private(iface);
        }


        const int Ncell = GetCellCount();
        m_cell_volume.resize(Ncell, 0.);
        m_cell_center.resize(Ncell);
        for (int icell=0;icell<Ncell;icell++){
            m_cell_center[icell] = CellGetCenter_private(icell);
        }
        for (int icell=0;icell<Ncell;icell++){
            m_cell_volume[icell] = CellGetVolume_private(icell);
        }
        




        
    }



bool foam_mesh::check_mesh(){
    /*
    *check Gauss theorem        *
    * 
    */
    const int Ncell = GetCellCount();
    for (int icell = 0;icell<Ncell;icell++){
        std::set<int> facelist = CellGetFaceList(icell);
        std::array<double,3> err3;
        err3[0] = 0.; err3[1] = 0.; err3[2] = 0.;
        for (const int iface:facelist){
            std::array<double,3> nface = FaceGetnorm(iface);
            double Sface = FaceGetSquare(iface);
            std::optional<int> owner;
            std::optional<int> neib;
            GetFace2OwnerNeibCell(iface,owner, neib);
            if (*owner == icell)
                err3 = err3 + nface*Sface;
                else 
                err3 = err3 - nface*Sface;
        }

        if ( (fabs(err3[0]) > 1e-3) || (fabs(err3[1]) > 1e-3 ) ||  (fabs(err3[2]) > 1e-3 ))
        {
            assert(false);
            return false;
        } 
    }
    return true;
}