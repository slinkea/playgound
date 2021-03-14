#include <iostream>
#include <cuda_runtime.h>
#include "CudaKernels.cuh"


__global__ void Kernel(int n, short* a)
{
  int tid = blockIdx.x * blockDim.x + threadIdx.x;

  if (tid < n) {
    a[tid]++;
  }
}

CudaKernel::CudaKernel()
{
  std::cout << "Reading CUDA-capable GPUs properties... " << std::endl;

  int deviceQty{};
  bool success{};
  if (cudaGetDeviceCount(&deviceQty) == cudaError_t::cudaSuccess) {
    for (int deviceIdx{}; deviceIdx < deviceQty; deviceIdx++) {
      cudaDeviceProp prop;
      if (cudaGetDeviceProperties(&prop, deviceIdx) == cudaError_t::cudaSuccess) {
        std::cout << "Device Number: " << deviceIdx << std::endl;
        std::cout << "  Device name: " << prop.name << std::endl;
        std::cout << "  Compute capability: " << prop.major << "." << prop.minor << std::endl;
        std::cout << "  Memory Clock Rate (KHz): " << prop.memoryClockRate << std::endl;
        std::cout << "  Memory Bus Width (bits): " << prop.memoryBusWidth << std::endl;
        std::cout << "  Peak Memory Bandwidth (GB/s): " << (2.0 * prop.memoryClockRate * (prop.memoryBusWidth / 8) / 1.0e6) << std::endl;
        std::cout << "  Global Memory (Bytes): " << prop.totalGlobalMem << std::endl;
        std::cout << std::endl;

        success = true;
      }
    }
  }

  if (!success) {
    throw std::runtime_error("Error while using CUDA library.");
  }



  //int N = 20 * (1 << 20);
  //float* x, * y, * d_x, * d_y;
  //x = (float*)malloc(N * sizeof(float));
  //y = (float*)malloc(N * sizeof(float));

  //cudaMalloc(&d_x, N * sizeof(float));
  //cudaMalloc(&d_y, N * sizeof(float));

  //for (int i = 0; i < N; i++) {
  //  x[i] = 1.0f;
  //  y[i] = 2.0f;
  //}

  //cudaEvent_t start, stop;
  //cudaEventCreate(&start);
  //cudaEventCreate(&stop);

  //cudaMemcpy(d_x, x, N * sizeof(float), cudaMemcpyHostToDevice);
  //cudaMemcpy(d_y, y, N * sizeof(float), cudaMemcpyHostToDevice);

  //cudaEventRecord(start);

  //// Perform Kernel on 1M elements
  //Kernel<<<(N + 511) / 512, 512>>>(N, 2.0f, d_x, d_y);

  //cudaEventRecord(stop);

  //cudaMemcpy(y, d_y, N * sizeof(float), cudaMemcpyDeviceToHost);

  //cudaEventSynchronize(stop);
  //float milliseconds = 0;
  //cudaEventElapsedTime(&milliseconds, start, stop);

  //float maxError = 0.0f;
  //for (int i = 0; i < N; i++) {
  //  maxError = std::max(maxError, abs(y[i] - 4.0f));
  //}

  //std::cout << "Max error: " << maxError << std::endl;
  //std::cout << "Effective Bandwidth (GB/s): " << (N * 4 * 3 / milliseconds / 1e6) << std::endl;
}

void CudaKernel::MemoryAllocation()
{
  // Allocate pinned memory
  bool success{};
  if (cudaMallocHost(&m_memHost, 1 << 30) == cudaError_t::cudaSuccess) {
    if (cudaMalloc(&m_memDevice, 1 << 30) == cudaError_t::cudaSuccess) {
      success = true;
    }
  }

  if (!success) {
    throw std::runtime_error("Error while allocating memory.");
  }
}

void CudaKernel::CopyMemoryToDevice()
{
  cudaError_t error = cudaMemcpy(m_memDevice, m_memHost, 1 << 30, cudaMemcpyHostToDevice);
}

void CudaKernel::Process()
{
  int N = 1 << 30;
  int blockSize = 0;
  int gridSize = (N + 512 - 1) / 512;
  dim3 blocks(gridSize, 1, 1);
  dim3 threadPerBlock(blockSize, 1, 1);

  Kernel<<<blocks, threadPerBlock>>>(N, m_memDevice);
  if (cudaDeviceSynchronize() == cudaError_t::cudaSuccess) {
    std::cout << "done" << std::endl;

    //cudaMemcpy(h_c, d_c, bytes, cudaMemcpyDeviceToHost);
  }
}

CudaKernel::~CudaKernel()
{
  bool success{};
  if (cudaFreeHost(m_memHost) == cudaError_t::cudaSuccess) {
    if (cudaFree(m_memDevice) == cudaError_t::cudaSuccess) {
      success = true;
    }
  }

  if (!success) {
    std::cout << "Error while freeing memory." << std::endl;
  }
}
