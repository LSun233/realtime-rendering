#pragma once
#include<string>
#include<iostream>
#include"../Utilis/tinyXML/tinyxml.h"
#include<Vector>
#include"../type_define.h"
#include<sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/vec1.hpp> 
#include<map>
// �����������ָ��ַ��������ظ��������� 
std::vector<float> parseFloats(const std::string& text) {
    std::vector<float> values; std::istringstream iss(text);
    float value;
    while (iss >> value) {
        values.push_back(value);
    }
    return values;
}

// �����������ָ��ַ����������������� 
std::vector<int> parseInts(const std::string& text) {
    std::vector<int> values; std::istringstream iss(text);
    int value; while (iss >> value) {
        values.push_back(value);
    }
    return values;
}


void daeGeometries(TiXmlElement* root, std::vector<Vertex>& vertices, std::vector<int>& vertexIndices)
{
    // �ҵ�geometryԪ��
    TiXmlElement* geometry = root->FirstChildElement("library_geometries")->FirstChildElement("geometry");
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

    // ����Position���� 
    std::vector<float> positionValues = parseFloats(sources[0]->FirstChildElement("float_array")->GetText());
    for (size_t i = 0; i < positionValues.size(); i += 3) {
        Vertex vertex;
        vertex.Position = glm::vec3(positionValues[i], positionValues[i + 1], positionValues[i + 2]);
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
    TiXmlElement* controller = root->FirstChildElement("library_controllers")->FirstChildElement("controller");
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

    //// �����������ݣ�Color, TexCoords�ȣ��ķ������� // ... // ���������Ķ������� 
    //for (const auto& vertex : vertices) 
    //{ 
    //    std::cout << "Position: " << vertex.Position.x << " " << vertex.Position.y << " " << vertex.Position.z << std::endl;
    //    std::cout << "Normal: " << vertex.Normal.x << " " << vertex.Normal.y << " " << vertex.Normal.z << std::endl;
    //    std::cout << "BoneID: " << vertex.m_BoneIDs[0] << "  " << vertex.m_BoneIDs[1] << "  " << vertex.m_BoneIDs[2] << "  " << vertex.m_BoneIDs[3] << "  " << std::endl;
    //    std::cout<<"BoneWeight: "<< vertex.m_Weights[0] << "  " << vertex.m_Weights[1] << "  " << vertex.m_Weights[2] << "  " << vertex.m_Weights[3] << "  " << std::endl;
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

NodeData parseDAE(TiXmlElement* root) {

    TiXmlElement* rootElement = root->FirstChildElement("library_visual_scenes")->FirstChildElement("visual_scene")->FirstChildElement("node");
    return parseNode(rootElement);

}

glm::mat4 parseMatrix(const std::string& matrixText) {
    std::stringstream ss(matrixText);
    float matrixValues[16];
    for (int i = 0; i < 16; ++i) {
        ss >> matrixValues[i];
    }
    return glm::make_mat4(matrixValues);
}

void parseLibraryControllers(TiXmlElement* root, std::map<std::string, BoneInfo>& boneInfoMap) {

    TiXmlElement* libraryControllers = root->FirstChildElement("library_controllers");
    if (!libraryControllers) {
        return; 
    } 
    int boneIndex = 0;
    TiXmlElement* controller = libraryControllers->FirstChildElement("controller");
    while (controller) {
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
}

void daeRead(std::string xmlpath, std::vector<Vertex>& vertices, std::vector<int>& vertexIndices,NodeData& Noderoot, std::map<string, BoneInfo> m_BoneInfoMap){
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

    //�������β���
    daeGeometries(root, vertices, vertexIndices);

    //����visual_scenes���������ӽڵ����״��ϵ�Լ��ӽڵ㵽���ڵ��ת����ϵ


 


}
