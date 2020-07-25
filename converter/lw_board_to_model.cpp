#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;
/*
tungmodel {
  int modelCount
  model models[modelCount]
  vec3 float pos1 // have to figure out what this is
  vec3 float pos2 // have to figure out what this is
}

model {
  char id
  int vertexCount
  vec3 float vertices[vertexCount]
  int indexCount
  int indices[indexCount]
  int colorCount
  vec4 |color| colors[colorCount]
  int uvCount
  vec2 float uvs[uvCount]
}

grid texture epsilon = 0.5-(2*(0.5/16)) / 0.4375
*/

struct vertexinfo{
    #pragma pack(1)
    char id;
    #pragma pack(pop)
    int vertexCount;
};

//extract color information |RGBA one byte each| 
struct color
{
    int r : 8;
    int g : 8;
    int b : 8;
    int a : 8;
};


int export_file(int export_method, string path, int model_id, int vertexCount, vector<float> vertices, int indexCount, vector<int> faces, int colorcount , vector<color> colors, int uvCount, vector<float> uvs) {
    
    //prepare to open the export file
    ofstream file_out;
    int actual_file = 0;
    for (int i = 0; i < path.length(); i++) if (path[i] == 47) actual_file = i;
    path.erase(actual_file + 1);

    switch(export_method){
    //case 0 obj
    case 0:
        cout << " Exporting as Obj\n";
        //open the file and print ad [dab]
        file_out.open((path + "model" + to_string(model_id + 1) + ".obj").c_str());
        file_out << "# LogicWorld board converter | https://github.com/Red-3D/LW-board-to-obj |\n\n";

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
    default:
        return 103;
    }
    
    return 1;

}

// loads all data from the path and exports both models with the specified method
int convert_board(string path, int export_method) {

    //opens file
    ifstream file;
    file.open(path.c_str(), ios::binary);

    //check if file is open
    if (!file) {
        return 101;
    }

    //get modelCount (always = 2)
    int modelCount;
    file.read((char*)&modelCount, sizeof(int));
    if (modelCount != 2) return 102;
    cout << "model count: " << modelCount << endl;

    //iterate though both models
    for (int i = 0; i < 2; i++) {

        //get the Vertex Count
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
        cout << " faces: " << indexCount / 3 << endl;
        for (int n = 0; n < indexCount; n++) {
            file.read((char*)&face, sizeof(int));
            faces.push_back(face);
        }

        //read amount of colors, idfk probably not imporatant but have to do
        int colorCount;
        color Color;
        vector<color> Colors;
        file.read((char*)&colorCount, sizeof(int));
        cout << " colorCount: " << colorCount << endl;
        for (int n = 0; n < colorCount; n++) {
            file.read((char*)&Color, sizeof(color));
            Colors.push_back(Color);
        }

        //read the uvs
        int uvcount;
        float uv;
        vector<float> uvs;
        file.read((char*)&uvcount, sizeof(int));
        cout << " uvcount: " << uvcount << endl;
        for (int n = 0; n < uvcount * 2; n++) {
            file.read((char*)&uv, sizeof(float));
            uvs.push_back(uv);
        }

        int export_file_return_value = export_file(export_method, path, i, VertexInfo.vertexCount, verts, indexCount, faces, colorCount, Colors, uvcount, uvs);
        if(export_file_return_value != 1) return export_file_return_value;
        cout << "----------------------------\n";
    }

    file.close();

    return 1;

}

int main()
{
    //some variables
    string path;
    int return_code;
    int export_method;

    //get and format file path
    cout << "\nLW board to model 0.2\n\nfile path: ";
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

    //everything is fine, even when it is not
    return 1;
}
