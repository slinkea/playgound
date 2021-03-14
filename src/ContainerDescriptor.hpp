#pragma once
#include <memory>
#include <vector>


template <typename T>
class Container
{
public:
  Container(const Container&) = delete;
  Container& operator=(const Container&) = delete;

  Container() = default;
  virtual ~Container() = default;

  Container(Container&& other) noexcept
    : m_items(std::move(other.m_items))
  {
  }

  Container& operator=(Container&& other) noexcept
  {
    if (this != &other) {
      m_items = std::move(other.m_items);
    }

    return *this;
  }

protected:
  std::vector<std::unique_ptr<T>> m_items;
};

class ElementaryAscanDescriptor
{
public:
  ElementaryAscanDescriptor(const ElementaryAscanDescriptor&) = delete;
  ElementaryAscanDescriptor& operator=(const ElementaryAscanDescriptor&) = delete;

  ElementaryAscanDescriptor() = default;
  ~ElementaryAscanDescriptor() = default;
  
  ElementaryAscanDescriptor(size_t v)
    : m_value(v)
  {
  }

  ElementaryAscanDescriptor(ElementaryAscanDescriptor&& other)
    : m_value(other.m_value)
  {
  }

  ElementaryAscanDescriptor& operator=(ElementaryAscanDescriptor&& other) noexcept
  {
    if (this != &other)
    {
      m_value = std::move(other.m_value);
    }

    return *this;
  }

  size_t m_value{};
};


class ElementaryAscanDescriptors : public Container<ElementaryAscanDescriptor>
{
public:
  ElementaryAscanDescriptors(const ElementaryAscanDescriptors&) = delete;
  ElementaryAscanDescriptors& operator=(const ElementaryAscanDescriptors&) = delete;

  ElementaryAscanDescriptors() 
  {
    m_items.push_back(std::make_unique<ElementaryAscanDescriptor>(911));
  }

  ~ElementaryAscanDescriptors() {}

  ElementaryAscanDescriptors(ElementaryAscanDescriptors&& other) noexcept
    : Container<ElementaryAscanDescriptor>(std::move(other))
  {
  }

  ElementaryAscanDescriptors& operator=(ElementaryAscanDescriptors&& other) noexcept
  {
    if (this != &other)
    {
      Container<ElementaryAscanDescriptor>::operator=(std::move(other));
    }

    return *this;
  }
};

class CycleDataDescriptor
{
public:
  CycleDataDescriptor() {}
  ~CycleDataDescriptor() {}

  CycleDataDescriptor(const CycleDataDescriptor&) = delete;
  CycleDataDescriptor& operator=(const CycleDataDescriptor&) = delete;

  CycleDataDescriptor(CycleDataDescriptor&& other)  noexcept
    : m_eads(std::move(other.m_eads))
  {
  }

  CycleDataDescriptor& operator=(CycleDataDescriptor&& other) noexcept
  {
    if (this != &other)
    {
      m_eads = std::move(other.m_eads);
    }

    return *this;
  }

private:
  ElementaryAscanDescriptors m_eads;
};
