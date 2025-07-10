#include<iostream>
#include <cuda_runtime.h> 
#include <device_launch_parameters.h>

/*核函数（设备运行函数）*/
__global__ void vectorAdd(const float* A, const float* B, float* C, int numElements)
{
    int i = blockDim.x * blockIdx.x + threadIdx.x;
    if (i < numElements)
    {
        C[i] = A[i] + B[i] + 10;
    }
}

/*主机函数*/
void test1(int num)
{
    /*生成主机数组内存 h_A, h_B, h_C*/
    int numElements = num;
    size_t size = numElements * sizeof(float);
    float* h_A = (float*)malloc(size);
    float* h_B = (float*)malloc(size);
    float* h_C = (float*)malloc(size);
    for (int i = 0; i < numElements; ++i)
    {
        h_A[i] = rand() / (float)RAND_MAX;
        h_B[i] = rand() / (float)RAND_MAX;
    }

    /*生成设备内存 d_A，d_B，d_C */
    float* d_A = NULL;
    cudaMalloc((void**)&d_A, size);
    float* d_B = NULL;
    cudaMalloc((void**)&d_B, size);
    float* d_C = NULL;
    cudaMalloc((void**)&d_C, size);

    /*将主机内存数据复制到设备内存 h_A--d_A，h_B--d_B */
    cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);

    /*设置设备的线程数，并调用核函数*/
    int threadsPerBlock = 256;
    int blocksPerGrid = (numElements + threadsPerBlock - 1) / threadsPerBlock;
    vectorAdd << < blocksPerGrid, threadsPerBlock >> > (d_A, d_B, d_C, numElements);
    cudaGetLastError();

    /*将设备内存数据复制到主机内存 d_C--h_C */
    cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);

    /* 释放设备内存 d_A d_B d_C */
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    /* 结果验证 */
    std::cout << "A[0]: " << (float)h_A[0] << " B[0]: " << (float)h_B[0] << " 结果C[0] = A[i] + B[i] + 10: " << (float)h_C[0] << std::endl;
    std::cout << "A[1]: " << (float)h_A[1] << " B[1]: " << (float)h_B[1] << " 结果C[1] = A[i] + B[i] + 10: " << (float)h_C[1] << std::endl;
    std::cout << "A[2]: " << (float)h_A[2] << " B[2]: " << (float)h_B[2] << " 结果C[2] = A[i] + B[i] + 10: " << (float)h_C[2] << std::endl;

    /* 释放主机内存 h_A h_B h_C */
    free(h_A);
    free(h_B);
    free(h_C);

}