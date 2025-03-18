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
            void creat(int N);  //叶子系节点的三角面片数量;
            HitResult hit(Ray ray);
            ~BVH();

       private:
                // 构建 BVH
                BVHNode* buildBVH(std::vector<Triangle>& triangles, int l, int r, int n);
                // 和 aabb 盒子求交，没有交点则返回 -1
                float hitAABB(Ray r, glm::vec3 AA, glm::vec3 BB);

                // 光线和三角形求交 
                float hitTriangle(Triangle* triangle, Ray ray);
                // 暴力查数组
                HitResult hitTriangleArray(Ray ray, std::vector<Triangle>& triangles, int l, int r);

                // 在 BVH 上遍历求交
                HitResult hitBVH(Ray ray, std::vector<Triangle>& triangles, BVHNode* root);


    };
}









