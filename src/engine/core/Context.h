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
      componentManager_->entityDeleted(entity);
      systemManager_->entityDeleted(entity);
    }

    // Component
    template<typename T>
    void registerComponent() {
      componentManager_->registerComponent<T>();
    }

    template<typename T>
    void addComponent(Entity entity, T component) {
      componentManager_->addComponent<T>(entity, component);

      auto signature = entityManager_->getSignature(entity);
      signature.set(componentManager_->getComponentType<T>(), true);
      entityManager_->setSignature(entity, signature);

      systemManager_->entitySignatureChanged(entity, signature);
    }

    template<typename T>
    void removeComponent(Entity entity) {
      componentManager_->removeComponent<T>(entity);

      auto signature = entityManager_->getSignature(entity);
      signature.set(componentManager_->getComponentType<T>(), false);
      entityManager_->setSignature(entity, signature);

      systemManager_->entitySignatureChanged(entity, signature);
    }

    template<typename T>
    T& getComponent(Entity entity) {
      return componentManager_->getComponent<T>(entity);
    }

    // System
    template<typename T>
    std::shared_ptr<T> registerSystem() {
      return systemManager_->registerSystem<T>();
    }

    template<typename T>
    void setSystemSignature(Signature signature) {
      systemManager_->setSignature<T>(signature);
    }

  private:
    std::unique_ptr<ComponentManager> componentManager_;
    std::unique_ptr<EntityManager> entityManager_;
    std::unique_ptr<SystemManager> systemManager_;
};
