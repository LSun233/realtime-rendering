#pragma once
#ifdef EXPORTEDCLASSES_EXPORTS
#define EXPORTEDCLASSES_API __declspec(dllexport)
#else
#define EXPORTEDCLASSES_API __declspec(dllimport)
#endif
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

struct Vertex;
void EXPORTEDCLASSES_API plyRead(std::string path, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

