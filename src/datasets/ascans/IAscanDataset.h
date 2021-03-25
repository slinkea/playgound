#pragma once
#include <memory>


class DatasetProperties;
struct AscanAttributes;
struct DataDimensions;

class IAscanDataset
{
public:
  virtual ~IAscanDataset() = default;
  
  virtual const DatasetProperties& Properties() const = 0;
  virtual const AscanAttributes& Attributes() const = 0;
  virtual const DataDimensions& Dimensions() const = 0;
  virtual void SelectSingle(size_t x_, size_t y_) const = 0;
  virtual void Read(void* dataOut_) const = 0;
  virtual bool IsStatus() const = 0;
  virtual bool IsData() const = 0;
  
};

using TIAscanDatasetPtr = std::shared_ptr<const IAscanDataset>;