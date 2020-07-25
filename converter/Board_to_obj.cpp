#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
/*
tungmodel {
  int modelCount X
  model models[modelCount]
  vec3 pos1 // Probably camera or something
  vec3 pos2
}

model {
  char id X
  int vertexCount X
  vec3 vertices[vertexCount] X
  int indexCount X
  int indices[indexCount] X
  int colorCount
  vec4 colors[colorCount]
  int uvCount
  vec2 uvs[uvCount]
}

model count: 2
 ID: 1
 VertexCount: 1392
 faces: 696
 colorCount: 1392
 uvcount: 1392
----------------------------
 ID: 0
 VertexCount: 9898
 faces: 4968
 colorCount: 9898
 uvcount: 0
----------------------------

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

int main()
{
    //file paths
    string path = "D:/CPP/C++ Projects/LW-Board_to_model/boards/BIG BOY TRUCK/model.tungmodel";
    string path_out = path;

    //generate the folder of the model to be able to place the objs in it. | 47 = / |
    int actual_file = 0;
    for (int i = 0; i < path.length(); i++) if (path[i] == 47) actual_file = i;
    path_out.erase(actual_file+1);
    
    //creates file streams and opend the tungmodel
    ifstream file;
    ofstream file_out;
    file.open(path.c_str(), ios::binary);

    //check if file is open
    if (!file) {
        return 101;
    }

    //get modelCount (always = 2 but we get it anyway)
    int modelCount;
    file.read((char*)&modelCount, sizeof(int));
    cout << "model count: " << modelCount << endl;

    //iterate though both models
    for (int i = 0; i < modelCount; i++) {
        file_out.open((path_out + "model" + to_string(i + 1) + ".obj").c_str());
        file_out << "# tungmodel to obj converter | https://github.com/Red-3D/LW-board-to-obj |\n\n";
        if (i == 0) {
            file_out << "mtllib grid.mtl\nusemtl grid\no boards\n\n";
        }
        else
        {
            file_out << "o components\n\n";
        }

        //get the Vertex Count
        vertexinfo VertexInfo;
        file.read((char*)&VertexInfo, sizeof(vertexinfo));
        cout << " ID: " << VertexInfo.id + 0 << endl;
        cout << " VertexCount: " << VertexInfo.vertexCount << endl;

        //read the vertices and write them to the obj
        float cord;

        for (int n = 0; n < VertexInfo.vertexCount; n++) {
            file_out << "v";
            for (int j = 0; j < 3; j++) {
                file.read((char*)&cord, sizeof(float));
                file_out << " " << cord;
            }
            file_out << "\n";
        }

        //read faces and add them to the face vector for later
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
        file.read((char*)&colorCount, sizeof(int));
        cout << " colorCount: " << colorCount << endl;
        for (int n = 0; n < colorCount; n++) {
            file.read((char*)&Color, sizeof(color));
            // ima ignore colors for now
            //cout << "  R:" << Color.r << " G:" << Color.g << " B:" << Color.b << " A:" << Color.a << endl;
            //file_out << Color.r << " " << Color.g << " " << Color.b << " " << "255\n";
        }

        //read and write the uvs
        int uvcount;
        float uvs;
        file.read((char*)&uvcount, sizeof(int));
        cout << " uvcount: " << uvcount << endl;
        for (int n = 0; n < (uvcount); n++) {
            file.read((char*)&uvs, sizeof(float));
            file_out << "vt " << uvs;
            file.read((char*)&uvs, sizeof(float));
            file_out << " " << uvs << "\n";
        }

        //write the faces to the file after doing the uvs
        for (int n = 0; n < indexCount / 3; n++) {
            file_out << "f";
            for (int j = 0; j < 3; j++) {
                //duck obj, index + 1 it is
                file_out << " " << faces[(3 * n) + j] + 1;
                if (i == 0) {
                    file_out << "/" << faces[(3 * n) + j] + 1;
                }
            }
            file_out << "\n";

        }

        file_out.close();
        cout << "----------------------------\n";
        }
        
    
}