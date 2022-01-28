//-----------------------------------------------------------------------------
// Classe qui impl�mente le pattern Observer. On utilise cette classe pour
// d�coupler nos composantes sans �gards � l'ordre d'appel.
//
// Note: Il faut favoriser un argument complexe
//
// Ownership: S�bastien Vachon
//-----------------------------------------------------------------------------
#pragma once

#include <atomic>
#include <functional>
#include <map>
#include <shared_mutex>

namespace ONDTLib
{

  template <typename TPublisher, typename... Args>
  class Event
  {
  public:
    Event() : m_id(0) {}

    Event(Event const& other) : m_id(0) {}

    int Subscribe(TPublisher* object_, void (TPublisher::* method)(Args...))
    {
      return Subscribe([=](Args... args)
        {
          (object_->*method)(args...);
        });
    }

    int Subscribe(TPublisher* object_, void (TPublisher::* method)(Args...) const)
    {
      return Subscribe([=](Args... args)
        {
          (object_->*method)(args...);
        });
    }

    int Subscribe(std::function<void(Args...)> const& slot_) const
    {
      m_mutex.lock();
      m_slots.insert(std::make_pair(++m_id, slot_));
      m_mutex.unlock();

      return m_id;
    }

    void Unsubscribe(int id_) const
    {
      std::unique_lock lock(m_mutex);
      m_slots.erase(id_);
    }

    void Clear() const
    {
      std::unique_lock lock(m_mutex);
      m_slots.clear();
    }

    Event& operator=(Event const&)
    {
      Clear();
    }

    //Uniquement le publisher d'un event peut la notifier.
    friend TPublisher;
    void Notify(Args... p) const
    {
      //TODO[SVC][Prendre une copie de m_slots avant la notification pour qu'un subscriber puisse se d�souscrire durant une notification.]
      std::shared_lock lock(m_mutex);
      for (auto const& slot : m_slots)
      {
        slot.second(std::forward<Args>(p)...);
      }
    }

  private:
    //TODO[SVC][Remplacer par un vecteur de pair...]
    mutable std::map<int, std::function<void(Args...)>> m_slots;
    mutable std::atomic<int>                            m_id;
    mutable std::shared_mutex                           m_mutex;
  };

} //namespace ONDTLib
