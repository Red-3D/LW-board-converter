#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

struct vertexinfo{
    //chars have extra padding, removed by pack
    #pragma pack(1)
    char id;
    #pragma pack(pop)
    int vertexCount;
};

//extract color information |RGBA one byte each| 
struct color
{
    unsigned int r : 8;
    unsigned int g : 8;
    unsigned int b : 8;
    unsigned int a : 8;
};


int export_file(int export_method, string path, int model_id, int vertexCount, vector<float> vertices, int indexCount, vector<int> faces, int colorcount , vector<color> colors, int uvCount, vector<float> uvs) {
    
    //prepare to open the export file
    ofstream file_out;
    //find the path itself
    int actual_file = 0;
    for (int i = 0; i < path.length(); i++) if (path[i] == 47) actual_file = i;
    path.erase(actual_file + 1);

    switch(export_method){
    //case 0 obj
    case 0:
        cout << " Exporting as Obj\n";
        //open the file and print ad [dab]
        file_out.open((path + "model" + to_string(model_id + 1) + ".obj").c_str());
        file_out << "# LogicWorld board converter | https://github.com/Red-3D/LW-board-converter |\n\n";

        //if the model number is 0 the mesh is named boards and assign the grid material, else the mesh will be named components
        if (model_id == 0) {
            file_out << "mtllib grid.mtl\nusemtl grid\no boards\n\n";
        }
        else
        {
            file_out << "o components\n\n";
        }

        //write vertices to file
        for (int i = 0; i < vertexCount; i++) {
            file_out << "v";
            for (int j = 0; j < 3; j++) {
                file_out << " " << vertices[(3*i) + j];
            }
            file_out << "\n";
        }
        
        //write uvs to file
        for (int i = 0; i < uvCount; i++) {
            file_out << "vt " << uvs[i * 2];
            file_out << " " << uvs[i * 2 + 1] << "\n";
        }

        //write faces to file
        for (int i = 0; i < indexCount / 3; i++) {
            file_out << "f";
            for (int j = 0; j < 3; j++) {
                file_out << " " << faces[(3 * i) + j] + 1;
                //if the model id is 0 the faces also get their uvs
                if (model_id == 0) {
                    file_out << "/" << faces[(3 * i) + j] + 1;
                }
            }
            file_out << "\n";
        }

        //close file duh
        file_out.close();
        return 1;
    case 1:
        cout << " Exporting as Ply\n";
        //open the file and print ad [dab], also some other stuff the file needs
        file_out.open((path + "model" + to_string(model_id + 1) + ".ply").c_str());
        file_out << "ply\nformat ascii 1.0\n";
        file_out << "comment LogicWorld board converter | https://github.com/Red-3D/LW-board-converter |\n";
        file_out << "element vertex " << vertexCount << "\n";
        file_out << "property float x\nproperty float y\nproperty float z\n";
        //if the model id is 0 the uv properties get added
        if (model_id == 0)file_out << "property float s\nproperty float t\n";
        file_out << "property uchar red\nproperty uchar green\nproperty uchar blue\nproperty uchar alpha\n";
        file_out << "element face " << indexCount / 3 << "\n";
        file_out << "property list uchar uint vertex_indices\nend_header\n";
        
        //vertex loop
        for (int i = 0; i < vertexCount; i++) {
            //vertex positions
            for (int j = 0; j < 3; j++) {
                file_out << vertices[(3 * i) + j] << " ";
            }
            //uvs
            if (model_id == 0) {
                file_out << uvs[i * 2] << " " << uvs[i * 2 + 1] << " ";
            }

            //vertex colors
            for (int j = 0; j < 4; j++) {
                switch (j) {
                case 0:
                    //red
                    file_out << colors[i].r << " ";
                    break;
                case 1:
                    //green
                    file_out << colors[i].g << " ";
                    break;
                case 2:
                    //blue
                    file_out << colors[i].b << " ";
                    break;
                case 3:
                    //alpha
                    file_out << colors[i].a << " ";
                    break;
                }
            }
            file_out << "\n";
        }

        //face loop
        for (int i = 0; i < indexCount / 3; i++) {
            //vertex count of faces = 3
            file_out << 3 << " ";
            for (int j = 0; j < 3; j++) {
                file_out << faces[(3 * i) + j] << " ";
            }
            file_out << "\n";
        }

        //close the file duh
        file_out.close();
        return 1;
    default:
        //unknown export method
        return 103;
    }
    
    //just in case
    return 1;

}

// loads all data from the path and exports both models with the specified method
int convert_board(string path, int export_method) {

    //opens file
    ifstream file;
    file.open(path.c_str(), ios::binary);

    //check if file is open
    if (!file) {
        //unable to open file
        return 101;
    }

    //get modelCount (always = 2)
    int modelCount;
    file.read((char*)&modelCount, sizeof(int));
    //if this is not 2 the user probably selected a wrong file
    if (modelCount != 2) return 102;
    cout << "\nmodel count: " << modelCount << endl;

    //iterate though both models
    for (int i = 0; i < 2; i++) {

        //get the Vertex info
        vertexinfo VertexInfo;
        file.read((char*)&VertexInfo, sizeof(vertexinfo));
        cout << " ID: " << VertexInfo.id + 0 << endl;
        cout << " VertexCount: " << VertexInfo.vertexCount << endl;

        //read the vertices
        float cord;
        vector<float> verts;
        for (int n = 0; n < VertexInfo.vertexCount * 3; n++) {
            file.read((char*)&cord, sizeof(float));
            verts.push_back(cord);
        }

        //read faces
        int indexCount;
        int face;
        vector<int> faces;
        file.read((char*)&indexCount, sizeof(int));
        cout << " faces: " << indexCount << endl;
        for (int n = 0; n < indexCount; n++) {
            file.read((char*)&face, sizeof(int));
            faces.push_back(face);
        }

        //read vertex colors
        int colorCount;
        color Color;
        vector<color> Colors;
        file.read((char*)&colorCount, sizeof(int));
        cout << " colorCount: " << colorCount << endl;
        for (int n = 0; n < colorCount; n++) {
            file.read((char*)&Color, sizeof(color));
            Colors.push_back(Color);
        }

        //read uvs
        int uvcount;
        float uv;
        vector<float> uvs;
        file.read((char*)&uvcount, sizeof(int));
        cout << " uvcount: " << uvcount << endl;
        for (int n = 0; n < uvcount * 2; n++) {
            file.read((char*)&uv, sizeof(float));
            uvs.push_back(uv);
        }

        //export the file
        int export_file_return_value = export_file(export_method, path, i, VertexInfo.vertexCount, verts, indexCount, faces, colorCount, Colors, uvcount, uvs);
        //the return value of the exporter is passed on to main
        if(export_file_return_value != 1) return export_file_return_value;
        cout << "----------------------------\n";
    }

    //close the file duh
    file.close();

    return 1;

}

int main()
{
    //user input
    string path;
    int return_code;
    int export_method;

    //get and format file path
    cout << "\nLW board converter 1.0\n\nfile path: ";
    getline(cin, path);
    replace(path.begin(), path.end(), '\\', '/');
    path.erase(remove(path.begin(), path.end(), '"'), path.end());

    //get export format
    cout << "\nConvert to: [0]obj [1]ply: ";
    cin >> export_method;
    
    //run the tool
    return_code = convert_board(path, export_method);

    // 10/10 error handeling
    switch (return_code){
    case 1:
        break;
    case 101:
        cout << "\n------------------\n";
        cout << "unable to open file: " << path;
        cout << "\n------------------\n";
        break;
    case 102:
        cout << "\n----------------------------------------------------------------\n";
        cout << "model number is not 2, make sure that the correct file is selected";
        cout << "\n----------------------------------------------------------------\n";
        break;
    case 103:
        cout << "\n------------------------------\n";
        cout << "could not find selected encoding";
        cout << "\n------------------------------\n";
        break;
    default:
        cout << "\n--------------------------------------------\n";
        cout << "you ducked up big time, return code: " << return_code << " unknown.";
        cout << "\n--------------------------------------------\n";
        break;
    }

    system("pause");

    //everything is fine, even when it is not
    return 1;
}
