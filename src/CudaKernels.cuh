#pragma once


class CudaKernel
{
public:
	CudaKernel();
	~CudaKernel();

	short* HostMemory() {
		return m_memHost;
	}

	short* DeviceMemory() {
		return m_memDevice;
	}

	void MemoryAllocation();
	void CopyMemoryToDevice();
	void Process();

private:
	short* m_memHost{};
	short* m_memDevice{};
};
