#include<iostream>
#include <cuda_runtime.h> 
#include <device_launch_parameters.h>

/*�˺������豸���к�����*/
__global__ void vectorAdd(const float* A, const float* B, float* C, int numElements)
{
    int i = blockDim.x * blockIdx.x + threadIdx.x;
    if (i < numElements)
    {
        C[i] = A[i] + B[i] + 10;
    }
}

/*��������*/
void test1(int num)
{
    /*�������������ڴ� h_A, h_B, h_C*/
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

    /*�����豸�ڴ� d_A��d_B��d_C */
    float* d_A = NULL;
    cudaMalloc((void**)&d_A, size);
    float* d_B = NULL;
    cudaMalloc((void**)&d_B, size);
    float* d_C = NULL;
    cudaMalloc((void**)&d_C, size);

    /*�������ڴ����ݸ��Ƶ��豸�ڴ� h_A--d_A��h_B--d_B */
    cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);

    /*�����豸���߳����������ú˺���*/
    int threadsPerBlock = 256;
    int blocksPerGrid = (numElements + threadsPerBlock - 1) / threadsPerBlock;
    vectorAdd << < blocksPerGrid, threadsPerBlock >> > (d_A, d_B, d_C, numElements);
    cudaGetLastError();

    /*���豸�ڴ����ݸ��Ƶ������ڴ� d_C--h_C */
    cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);

    /* �ͷ��豸�ڴ� d_A d_B d_C */
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    /* �����֤ */
    std::cout << "A[0]: " << (float)h_A[0] << " B[0]: " << (float)h_B[0] << " ���C[0] = A[i] + B[i] + 10: " << (float)h_C[0] << std::endl;
    std::cout << "A[1]: " << (float)h_A[1] << " B[1]: " << (float)h_B[1] << " ���C[1] = A[i] + B[i] + 10: " << (float)h_C[1] << std::endl;
    std::cout << "A[2]: " << (float)h_A[2] << " B[2]: " << (float)h_B[2] << " ���C[2] = A[i] + B[i] + 10: " << (float)h_C[2] << std::endl;

    /* �ͷ������ڴ� h_A h_B h_C */
    free(h_A);
    free(h_B);
    free(h_C);

}