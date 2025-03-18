#pragma once
#include"type_define.h"
#include<vector>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



namespace BVH
{


    class BVH
    {
        public:
            BVHNode* BVHRoot;
            std::vector<Triangle> Triangles;
        private:
        
       
        public:
            BVH(const vector<Vertex>& vertices, const vector<unsigned int>& indices);
            void creat(int N);  //Ҷ��ϵ�ڵ��������Ƭ����;
            HitResult hit(Ray ray);
            ~BVH();

       private:
                // ���� BVH
                BVHNode* buildBVH(std::vector<Triangle>& triangles, int l, int r, int n);
                // �� aabb �����󽻣�û�н����򷵻� -1
                float hitAABB(Ray r, glm::vec3 AA, glm::vec3 BB);

                // ���ߺ��������� 
                float hitTriangle(Triangle* triangle, Ray ray);
                // ����������
                HitResult hitTriangleArray(Ray ray, std::vector<Triangle>& triangles, int l, int r);

                // �� BVH �ϱ�����
                HitResult hitBVH(Ray ray, std::vector<Triangle>& triangles, BVHNode* root);


    };
}









