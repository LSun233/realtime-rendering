#include"daeRead.h"
#include"../Animation/animation.h"
#include"../Animation/Bone.h"
#include"../mesh/mesh.h"
#include"../Utilis/tinyXML/tinyxml.h"

// �����������ָ��ַ��������ظ��������� 
std::vector<float> parseFloats(const std::string& text) {
    std::vector<float> values; std::istringstream iss(text);
    float value;
    while (iss >> value) {
        values.push_back(value);
    }
    return values;
}
glm::mat4 parseMatrix(const std::string& matrixText) {
    std::stringstream ss(matrixText);
    float matrixValues[16];
    for (int i = 0; i < 16; ++i) {
        ss >> matrixValues[i];
    }
    return glm::make_mat4(matrixValues);
}
// �����������ָ��ַ����������������� 
std::vector<int> parseInts(const std::string& text) {
    std::vector<int> values; std::istringstream iss(text);
    int value; while (iss >> value) {
        values.push_back(value);
    }
    return values;
}


void parseGeometries(TiXmlElement* root, float unitMeter, vector<Mesh*>& meshes)
{
    // �ҵ����е�geometryԪ��

        // ��ȡ����sourceԪ�� 
    std::vector<TiXmlElement*> all_geometry;
    for (TiXmlElement* geometry = root->FirstChildElement("library_geometries")->FirstChildElement("geometry"); geometry != nullptr; geometry = geometry->NextSiblingElement("geometry")) {
        all_geometry.push_back(geometry);
    }

    for (int geometry_index = 0; geometry_index < all_geometry.size(); geometry_index++)
    {
        TiXmlElement* geometry = all_geometry[geometry_index];
        if (geometry == nullptr) {
            std::cerr << "Failed to find geometry element" << std::endl;
            return;
        }

        // �ҵ�meshԪ�ز������������� 
        TiXmlElement* mesh = geometry->FirstChildElement("mesh");


        // ��ȡ����sourceԪ�� 
        std::vector<TiXmlElement*> sources;
        for (TiXmlElement* source = mesh->FirstChildElement("source"); source != nullptr; source = source->NextSiblingElement("source")) {
            sources.push_back(source);
        }


        std::vector<Vertex> vertices;
        std::vector<unsigned int> vertexIndices;

        // ����Position���� 
        std::vector<float> positionValues = parseFloats(sources[0]->FirstChildElement("float_array")->GetText());
        for (size_t i = 0; i < positionValues.size(); i += 3) {
            Vertex vertex;
            vertex.Position = glm::vec3(unitMeter * positionValues[i], unitMeter * positionValues[i + 1], unitMeter * positionValues[i + 2]);
            vertices.push_back(vertex);
        }
        // ����Normal���� 
        std::vector<float> normalValues = parseFloats(sources[1]->FirstChildElement("float_array")->GetText());
        // ����TexCoords����
        std::vector<float> texCoordValues = parseFloats(sources[2]->FirstChildElement("float_array")->GetText());
        // ����polylistԪ�ز�ƥ�䶥�㡢���ߺ���������
        TiXmlElement* polylist = mesh->FirstChildElement("polylist");
        TiXmlElement* p = polylist->FirstChildElement("p");
        std::vector<int> indices = parseInts(p->GetText());


        // ��ȡÿ�����ж��ٸ����� 
        TiXmlElement* vcount = polylist->FirstChildElement("vcount");
        std::vector<int> vcounts = parseInts(vcount->GetText());
        size_t index = 0; for (size_t i = 0; i < vcounts.size(); ++i) {
            for (int j = 0; j < vcounts[i]; ++j) {
                int vertexIndex = indices[index * 3];
                int texCoordIndex = indices[index * 3 + 2];
                // ��������Ľṹͨ���� ��������/������������/�������� 
                int normalIndex = indices[index * 3 + 1];
                vertices[vertexIndex].TexCoords = glm::vec2(texCoordValues[texCoordIndex * 2], texCoordValues[texCoordIndex * 2 + 1]);
                vertices[vertexIndex].Normal = glm::vec3(normalValues[normalIndex * 3], normalValues[normalIndex * 3 + 1], normalValues[normalIndex * 3 + 2]); ++index;

                vertexIndices.push_back(vertexIndex);    // �洢��Ƭ�Ķ���������vector<int>��
            }
        }
        // ��������Ȩ������
        // ����library_controllers�еĹ���Ȩ������ 
        std::vector<TiXmlElement*> controllers;
        for (TiXmlElement* controller = root->FirstChildElement("library_controllers")->FirstChildElement("controller"); controller != nullptr; controller = controller->NextSiblingElement("controller")) {
            controllers.push_back(controller);
        }

        TiXmlElement* controller = controllers[geometry_index];
        TiXmlElement* skin = controller->FirstChildElement("skin");
        TiXmlElement* vertexWeights = skin->FirstChildElement("vertex_weights");
        TiXmlElement* vcountElement = vertexWeights->FirstChildElement("vcount");
        TiXmlElement* vElement = vertexWeights->FirstChildElement("v");

        std::vector<int> vCounts = parseInts(vcountElement->GetText());
        std::vector<int> vValues = parseInts(vElement->GetText());
        // ����Ȩ������ 
        TiXmlElement* weightsSource = skin->FirstChildElement("source")->NextSiblingElement("source")->NextSiblingElement("source");
        std::vector<float> weights = parseFloats(weightsSource->FirstChildElement("float_array")->GetText());

        // ������ID��Ȩ�� 
        size_t vIndex = 0; for (size_t i = 0; i < vertices.size(); ++i) {
            int influenceCount = vCounts[i];
            for (int j = 0; j < influenceCount; ++j) {
                int jointId = vValues[vIndex++];
                int weightIndex = vValues[vIndex++];
                if (j < MAX_BONE_INFLUENCE) {
                    vertices[i].m_BoneIDs[j] = jointId;
                    vertices[i].m_Weights[j] = weights[weightIndex];
                }
            }
        }
        Mesh* meshInstance = new Mesh(vertices, vertexIndices);
        meshes.push_back(meshInstance);

    }
}
glm::mat4 parseTransformation(TiXmlElement* element)
{
    glm::mat4 transform(1.0f); // ��ʼ��Ϊ��λ���� 
    TiXmlElement* matrixElement = element->FirstChildElement("matrix");
    if (matrixElement) {
        std::string matrixText = matrixElement->GetText();
        std::stringstream ss(matrixText); float matrixValues[16];
        for (int i = 0; i < 16; ++i) {
            ss >> matrixValues[i];
        }
        transform = glm::make_mat4(matrixValues);
        return transform;
    }
    TiXmlElement* translateElement = element->FirstChildElement("translate");
    if (translateElement) {
        float x, y, z; translateElement->QueryFloatAttribute("x", &x);
        translateElement->QueryFloatAttribute("y", &y);
        translateElement->QueryFloatAttribute("z", &z);
        transform = glm::translate(transform, glm::vec3(x, y, z));
    }
    TiXmlElement* rotateElement = element->FirstChildElement("rotate");
    while (rotateElement) {
        float angle, x, y, z;
        rotateElement->QueryFloatAttribute("angle", &angle);
        rotateElement->QueryFloatAttribute("x", &x);
        rotateElement->QueryFloatAttribute("y", &y);
        rotateElement->QueryFloatAttribute("z", &z);
        transform = glm::rotate(transform, glm::radians(angle), glm::vec3(x, y, z));
        rotateElement = rotateElement->NextSiblingElement("rotate");
    }
    TiXmlElement* scaleElement = element->FirstChildElement("scale");
    if (scaleElement) {
        float x, y, z; scaleElement->QueryFloatAttribute("x", &x);
        scaleElement->QueryFloatAttribute("y", &y);
        scaleElement->QueryFloatAttribute("z", &z);
        transform = glm::scale(transform, glm::vec3(x, y, z));
    }
    return transform;
}
NodeData parseNode(TiXmlElement* nodeElement) {

    NodeData nodeData;
    nodeData.name = nodeElement->Attribute("name");
    nodeData.transformation = parseTransformation(nodeElement);
    TiXmlElement* childElement = nodeElement->FirstChildElement("node");
    while (childElement) {
        nodeData.children.push_back(parseNode(childElement));
        childElement = childElement->NextSiblingElement("node");
    }
    nodeData.childrenCount = nodeData.children.size();
    return nodeData;
}
NodeData parseLibraryScenes(TiXmlElement* root) {

    TiXmlElement* rootElement = root->FirstChildElement("library_visual_scenes")->FirstChildElement("visual_scene")->FirstChildElement("node");

    return parseNode(rootElement);
}

void parseLibraryControllers(TiXmlElement* root, float unitMeter, std::vector<std::map<string, BoneInfo>>& BoneInfoMaps) {

    TiXmlElement* libraryControllers = root->FirstChildElement("library_controllers");
    if (!libraryControllers) {
        return;
    }


    // ��ȡ����controllerԪ�� 
    std::vector<TiXmlElement*> controllers;
    for (TiXmlElement* controller = libraryControllers->FirstChildElement("controller"); controller != nullptr; controller = controller->NextSiblingElement("controller")) {
        controllers.push_back(controller);
    }


    for (int controller_index = 0; controller_index < controllers.size(); controller_index++)
    {
        int boneIndex = 0;
        std::map<string, BoneInfo>boneInfoMap;
        TiXmlElement* controller = controllers[controller_index];
        if (controller) {
            TiXmlElement* skin = controller->FirstChildElement("skin");
            if (skin) {
                TiXmlElement* bindShapeMatrixElement = skin->FirstChildElement("bind_shape_matrix");

                // ��ȡ����sourceԪ�� 0Ϊjoint_name,1ΪoffsetMatrices
                std::vector<TiXmlElement*> sources;
                for (TiXmlElement* source = skin->FirstChildElement("source"); source != nullptr; source = source->NextSiblingElement("source")) {
                    sources.push_back(source);
                }

                //����name
                TiXmlElement* nameArray = sources[0]->FirstChildElement("Name_array");
                std::vector<std::string> jointNames;
                std::string jointNamesStr = nameArray->GetText();
                std::istringstream nameStream(jointNamesStr);
                std::string name; while (nameStream >> name) {
                    jointNames.push_back(name);
                }

                //����offset
                TiXmlElement* floatArray = sources[1]->FirstChildElement("float_array");
                std::string matrixText = floatArray->GetText();
                std::istringstream matrixStream(matrixText);
                std::vector<glm::mat4> bindPoses;
                while (matrixStream) {
                    glm::mat4 matrix;
                    for (int i = 0; i < 16; ++i) {
                        matrixStream >> ((float*)&matrix)[i];
                    }

                    matrix[0][3] = unitMeter * matrix[0][3];
                    matrix[1][3] = unitMeter * matrix[1][3];
                    matrix[2][3] = unitMeter * matrix[2][3];



                    if (matrixStream) {
                        bindPoses.push_back(matrix);
                    }
                }

                // ��������Ϣ�洢��map�� 
                for (size_t i = 0; i < jointNames.size(); ++i) {
                    BoneInfo boneInfo;
                    boneInfo.id = boneIndex++;
                    boneInfo.offset = bindPoses[i];
                    boneInfoMap[jointNames[i]] = boneInfo;
                }
            }
        }
        // glm::mat4 mat = boneInfoMap["mixamorig_LeftUpLeg"].offset;
        BoneInfoMaps.push_back(boneInfoMap);
    }





}
void parseLibraryAnimation(TiXmlElement* root, float unitMeter, std::vector<Bone>& Bones)
{
    TiXmlElement* library_animations = root->FirstChildElement("library_animations");
    if (library_animations) {
        int id = 0;
        for (TiXmlElement* animation = library_animations->FirstChildElement("animation"); animation; animation = animation->NextSiblingElement("animation")) {
            std::string name = animation->Attribute("name");
            Bone bone(name, id++);

            // ��������������ʱ���
            //ʱ���
            std::vector<TiXmlElement*> sources;
            for (TiXmlElement* source = animation->FirstChildElement("source"); source != nullptr; source = source->NextSiblingElement("source")) {
                sources.push_back(source);
            }
            //const char* timestamp_data = sources[1]->FirstChildElement("float_array")->GetText();
            //std::vector<float> timeStamps; 
            //std::istringstream iss_ts(timestamp_data);

            //while (iss_ts >> value) {
            //    timeStamps.push_back(value);
            //}


            //����
            const char* data = sources[1]->FirstChildElement("float_array")->GetText();
            std::vector<float> values;
            std::istringstream iss(data);
            float value;
            while (iss >> value) {
                values.push_back(value);

            }


            float timeStep = 1 / 30.0f;


            // Convert the float values into 4x4 matrices 
            std::vector<std::vector<float>> matrices;
            for (size_t i = 0; i < values.size(); i += 16) {
                std::vector<float> matrix(values.begin() + i, values.begin() + i + 16);
                matrices.push_back(matrix);
            }

            for (int i = 0; i < matrices.size(); i++) {
                glm::mat4x4 matrix;
                for (int k = 0; k < 4; ++k) {
                    for (int j = 0; j < 4; ++j) {
                        matrix[k][j] = matrices[i][k * 4 + j];
                    }
                }

                float timeStamp = timeStep * (i + 1);
                glm::vec3 position = glm::vec3(unitMeter * matrix[0][3], unitMeter * matrix[1][3], unitMeter * matrix[2][3]);


                // ��ȡƽ��
                glm::quat rotation = glm::quat_cast(matrix);


                // ��ȡ��ת
                glm::vec3 scale(glm::length(glm::vec3(matrix[0])), glm::length(glm::vec3(matrix[1])), glm::length(glm::vec3(matrix[2])));
                // ��ȡ����
                KeyPosition keyPos{ position, timeStamp };
                KeyRotation keyRot{ rotation, timeStamp };
                KeyScale keyScl{ scale, timeStamp };


                bone.m_Positions.push_back(keyPos);
                bone.m_Rotations.push_back(keyRot);
                bone.m_Scales.push_back(keyScl);
            }

            bone.m_NumPositions = bone.m_Positions.size();
            bone.m_NumRotations = bone.m_Rotations.size();
            bone.m_NumScalings = bone.m_Scales.size();
            Bones.push_back(bone);
        }

        //for (int i = 0; i < Bones.size(); i++) {
        //
        //    for (int j = 0; j < Bones[i].m_NumPositions; j++)
        //    {
        //        std::cout << Bones[i].m_Positions[j].timeStamp<<"    "<< Bones[i].m_Positions[j].position.x << "  " << Bones[i].m_Positions[j].position.y << "  " << Bones[i].m_Positions[j].position.z << std::endl;


        //    }
        //}



    }
    else
    {
        std::cout << "cant find  animation" << std::endl;
    }
}


void printTree(NodeData node, int depth) {
    // Print the current node with indentation based on depth
    std::cout << depth << std::string(depth * 4, ' ') << node.name << std::endl;
    std::cout << " -------------------------------" << std::endl;
    for (int j = 0; j < 4; j++)
    {
        std::cout << std::string(depth * 4, ' ');
        for (int k = 0; k < 4; k++)
        {
            std::cout << node.transformation[j][k] << "     ";
        }
        std::cout << std::endl;

    }
    std::cout << " -------------------------------" << std::endl;
    // Recursively print the children 
    for (const auto& child : node.children)
    {
        printTree(child, depth + 1);
    }
}


void daeRead(std::string xmlpath, vector<Mesh*>& meshes, vector<Animation*>& animations) {
    TiXmlDocument doc(xmlpath.data());
    bool loadOkay = doc.LoadFile();
    if (!loadOkay)
    {
        std::cout << "����xml�ļ�ʧ��" << std::endl;
    }
    else
    {
        std::cout << "����xml�ļ��ɹ�" << std::endl;
    }
    if (!doc.LoadFile()) {
        std::cerr << "Failed to load file 'example.dae'" << std::endl;
        return;
    }

    TiXmlElement* root = doc.RootElement();
    if (root == nullptr) {
        std::cerr << "Failed to get root element" << std::endl; return;
    }


    const char* meter = root->FirstChildElement("asset")->FirstChildElement("unit")->Attribute("meter");
    float unitMeter = std::atof(meter) * 0.1;


    std::cout << "�������β���" << std::endl;
    parseGeometries(root, unitMeter, meshes);


    std::cout << "�����ڵ���" << std::endl;
    NodeData Noderoot;
    Noderoot = parseLibraryScenes(root);


    std::cout << "������������" << std::endl;
    std::vector<Bone> Bones;
    parseLibraryAnimation(root, unitMeter, Bones);


    std::cout << "��������������" << std::endl;
    std::vector<std::map<string, BoneInfo>> BoneInfoMaps;
    parseLibraryControllers(root, unitMeter, BoneInfoMaps);

    for (int i = 0; i < BoneInfoMaps.size(); i++)
    {
        std::map<string, BoneInfo>BoneInfoMap = BoneInfoMaps[i];
        Animation* anima = new Animation(Noderoot, Bones, BoneInfoMap);
        animations.push_back(anima);
    }
}
