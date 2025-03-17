#pragma once
#ifdef EXPORTEDCLASSES_EXPORTS
#define EXPORTEDCLASSES_API __declspec(dllexport)
#else
#define EXPORTEDCLASSES_API __declspec(dllimport)
#endif
#include<string>
#include<iostream>
#include<map>
#include<Vector>
#include<sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/vec1.hpp> 

#include"../type_define.h"
class TiXmlElement;
class Mesh;
class Animation;
class Bone;

// 帮助函数：分割字符串并返回浮点数向量 
std::vector<float> parseFloats(const std::string& text);
glm::mat4 parseMatrix(const std::string& matrixText);
// 帮助函数：分割字符串并返回整数向量 
std::vector<int> parseInts(const std::string& text);
void parseGeometries(TiXmlElement* root, float unitMeter, std::vector<Mesh*>& meshes);
glm::mat4 parseTransformation(TiXmlElement* element);
NodeData parseNode(TiXmlElement* nodeElement);
void parseLibraryControllers(TiXmlElement* root, float unitMeter, std::vector<std::map<std::string, BoneInfo>>& BoneInfoMaps);
void parseLibraryAnimation(TiXmlElement* root, float unitMeter, std::vector<Bone>& Bones);
void printTree(NodeData node, int depth = 0);
void EXPORTEDCLASSES_API daeRead(std::string xmlpath, std::vector<Mesh*>& meshes, std::vector<Animation*>& animations);
