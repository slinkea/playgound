#pragma once
#include "A.hpp"

class Playground
{
public:
	Playground();
	~Playground();

	Playground(const Playground&) = delete;
	Playground& operator=(const Playground&) = delete;

	// Move constructor.
	Playground(CycleDataDescriptor&&) noexcept;

	// Move assignment operator.
	Playground& operator=(Playground&& other) noexcept;

private:
	const CycleDataDescriptor m_cdd;
};
