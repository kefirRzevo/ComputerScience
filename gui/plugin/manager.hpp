#pragma once

#include "plugin.hpp"

template <class T> class Manager {
private:
  Manager() : active(nullptr) {}

  static std::unique_ptr<Manager<T>> manager;
  std::vector<std::unique_ptr<T>> objects;
  T *active;

public:
  ~Manager() = default;

  static Manager *Get() {
    if (manager.get() == nullptr)
      manager.reset(new Manager<T>());

    return manager.get();
  }

  T *GetActive() { return active; }

  void SetActive(T *object) { active = object; }

  void Add(T *object) { objects.push_back(object); }
};

template <class T> std::unique_ptr<Manager<T>> Manager<T>::manager;
