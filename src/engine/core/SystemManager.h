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

    void entitySignatureChanged(Entity entity, Signature entitySignature) {
      // Notify each system that an entity's signature changed
      for (auto const& pair : systems_) {
        auto const& type = pair.first;
        auto const& system = pair.second;
        auto const& systemSignature = signatures_[type];

        // Entity signature matches system signature - insert into set
        if ((entitySignature & systemSignature) == systemSignature) {
          system->entities().insert
        }
      }
    }

  private:
    std::unordered_map<const char*, Signature> signatures_{};
    std::unordered_map<const char*, std::shared_ptr<System>> systems_{};
};
