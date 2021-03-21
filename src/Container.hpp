#pragma once
#include <memory>
#include <vector>
#include <algorithm>
#include <functional>

//---------------------------------------------------------------------------------------
// Sébastien Vachon
//
// Classe pour faciliter la création de container d'item basé sur des unique_ptr.
//---------------------------------------------------------------------------------------
namespace ONDTLib
{
  template <typename T>
  class Container
  {
  public:
    using TSuper = ONDTLib::Container<T>;       //Pour que ca compile en cgg. __super ne fonctionne pas en gcc.

    using TItem = T;
    using TItemP = T*;
    using TItems = std::vector<TItemP>;
    using TItemPtr = std::unique_ptr<T>;
    using TItemPtrs = std::vector<TItemPtr>;
    using TFindPredicate = std::function<bool(const TItemPtr&)>;

    Container(const Container&) = delete;
    Container& operator=(const Container&) = delete;

    Container() = default;
    virtual ~Container() {}

    Container(Container&& other_) noexcept
      : m_items(std::move(other_.m_items))
    {
    }

    Container& operator=(Container&& other_) noexcept
    {
      m_items = std::move(other_.m_items);
      return *this;
    }

    T* Add(TItemPtr&& itemPtr_)
    {
      m_items.emplace_back(std::move(itemPtr_));
      return m_items.back().get();
    }

    const TItemPtrs& Items() const { return m_items; }

    void Clear()
    {
      m_items.clear();
    }

    TItemP Find(const TFindPredicate& predicate_) const
    {
      auto itr = std::find_if(m_items.begin(), m_items.end(), predicate_);
      return itr != m_items.end() ? (*itr).get() : nullptr;
    }

    TItems Select(const TFindPredicate& predicate_) const
    {
      TItems selected;

      auto itr = std::find_if(m_items.begin(), m_items.end(), predicate_);
      while (itr != m_items.end())
      {
        selected.push_back((*itr).get());
        itr = std::find_if(std::next(itr), m_items.end(), predicate_);
      }

      return selected;
    }

    void Erase(const TFindPredicate& predicate_)
    {
      m_items.erase(std::remove_if(m_items.begin(), m_items.end(), predicate_), m_items.end());
    }

    size_t Count(const TFindPredicate& predicate_) const
    {
      return std::count_if(m_items.begin(), m_items.end(), predicate_);
    }

    size_t Count() const
    {
      return m_items.size();
    }

  protected:
    TItemPtrs m_items;
  };
} //ONDTLib