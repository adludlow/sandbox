#pragma once

#include <unordered_map>
#include <memory>
#include <cassert>

#include "types.h"

class SystemManager {
  public:
    template<typename T>
    std::shared_ptr<T> registerSystem() {
      const char* typeName = typeid(T).name();

      assert(systems_.find(typeName) == systems_.end() && "Attempted to register system more than once.");

      auto system = std::make_shared<T>();
      systems_.insert({ typeName, system });
      return system;
    }

    template<typename T>
    void setSignature(Signature signature) {
      const char* typeName = typeid(T).name();

      assert(systems_.find(typeName) != systems_.end() && "System not registered.");

      signatures_.insert({ typeName, signature });
    }

    template<typename T>
    void entityDestroyed(Entity entity) {
      for (auto const& pair : systems_) {
        auto const& system = pair.second;
        system->entities().erase(entity);
      }
    }

  private:
    std::unordered_map<const char*, Signature> signatures_{};
    std::unordered_map<const char*, std::shared_ptr<System>> systems_{};
};
