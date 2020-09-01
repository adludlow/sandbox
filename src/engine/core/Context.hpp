#pragma once

#include <memory>

#include "ComponentManager.h"
#include "EntityManager.h"
#include "SystemManager.h"

class Context {
  public:
    void init() {
      componentManager_ = std::make_unique<ComponentManager>();
      entityManager_ = std::make_unique<EntityManager>();
      systemManager_ = std::make_unique<SystemManager>();
    }

    // Entity
    Entity createEntity() {
      return entityManager_->createEntity();
    }

    void deleteEntity(Entity entity) {
      entityManager_->deleteEntity(entity);
      componentManager_->deleteEntity(entity);
      systemManager_->deleteEntity(entity);
    }

    // Component
    template<typename T>
    void registerComponent() {
      componentManager_->registerComponent<T>();
    }

    template<typename T>
    void addComponent(Entity entity, T component) {
      componentManager_->addComponent<T>(entity, component);

      auto signature = entityManager->getSignature(entity);
      signature.set(componentManager->getComponentType<T>(), true);
      entityManager_->setSignature(entity, signature);

      systemManager_->entity
    }

  private:
    std::unique_ptr<ComponentManager> componentManager_;
    std::unique_ptr<EntityManager> entityManager_;
    std::unique_ptr<SystemManager> systemManager_;
};
