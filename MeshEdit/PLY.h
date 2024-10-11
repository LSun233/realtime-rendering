#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include"type_define.h"

void plyRead(std::string path, std::vector<Vertex>& vertices, std:: vector<unsigned int>& indices)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file." << std::endl;
        return ;
    }


    std::string line;
    int vertexCount = 0,faceCount = 0;
    string temp;
    bool flagNormal = false;
    bool flagColor = false;

    // 读取头部信息
    while (std::getline(file, line)) {
        if (line.find("end_header") != std::string::npos) {
            break;
        }
        if (line.find("element vertex") != std::string::npos) {
            std::istringstream iss(line);
            iss >> temp>> temp >> vertexCount;
        }
        else if (line.find("element face") != std::string::npos) {
            std::istringstream iss(line);
            iss >> temp >> temp >> faceCount;
        }
        if (line.find("property float nx") != std::string::npos)  flagNormal = true;
        if (line.find("property uchar red") != std::string::npos)  flagColor = true;
    }

    // 读取顶点属性
    for (int i = 0; i < vertexCount;i++)
    {
        std::getline(file, line);
        std::istringstream iss(line);
        Vertex vertex;
        iss >> vertex.Position[0] >> vertex.Position[1] >> vertex.Position[2];
  /*      vertex.Position[0]= 1000 * vertex.Position[0];
        vertex.Position[1] = 1000 * vertex.Position[1];
        vertex.Position[2] = 1000 * vertex.Position[2];*/
        if (flagNormal)
        {
            iss >> vertex.Normal[0] >> vertex.Normal[1] >> vertex.Normal[2];
        }
        if (flagColor)
        {
            iss >> vertex.Color[0] >> vertex.Color[1] >> vertex.Color[2];
        }
        vertices.push_back(vertex);
    }

    // 读取面片索引
    for (int i = 0; i < faceCount; ++i) {
       std::getline(file, line);
       std::istringstream iss(line);
       int index;
       int i0, i1, i2;
       iss >> index>> i0>> i1>>i2;
       indices.push_back(i0);
       indices.push_back(i1);
       indices.push_back(i2);
    } 
    // 关闭文件
    file.close();

}

