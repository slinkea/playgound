#pragma once
#include "ContainerDescriptor.hpp"

class Playground
{
public:

	Playground(const Playground&) = delete;
	Playground& operator=(const Playground&) = delete;

	Playground();
	~Playground();

	// Move constructor.
	Playground(CycleDataDescriptor&&) noexcept;

	// Move assignment operator.
	Playground& operator=(Playground&& other) noexcept;

private:
	const CycleDataDescriptor m_cdd;
};
