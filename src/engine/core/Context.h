#ifndef CONTEXT_H
#define CONTEXT_H

#include <memory>
#include <functional>

#include "ComponentManager.h"
#include "EntityManager.h"
#include "SystemManager.h"
#include "ResourceManager.h"
#include "./input/InputHandler.h"

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

    Entity createEntity(const std::string& name) {
      return entityManager_->createEntity(name);
    }

    void deleteEntity(Entity entity) {
      entityManager_->deleteEntity(entity);
      componentManager_->entityDeleted(entity);
      systemManager_->entityDeleted(entity);
    }

    Entity getNamedEntity(const std::string& name) {
      return entityManager_->getNamedEntity(name);
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

    template<typename T>
    ComponentType getComponentType() {
      return componentManager_->getComponentType<T>();
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

    // Input
    void registerInputHandler(std::shared_ptr<InputHandler> inputHandler) {
      inputHandler_ = inputHandler;
    }

    void update(float dt) {
      for (auto pre : preUpdateHandlers_) {
        pre();
      }

      systemManager_->update(dt);

      for (auto post : postUpdateHandlers_) {
        post();
      }
    }

    void addPostUpdateHandler(std::function<void()> callback) {
      postUpdateHandlers_.push_back(callback);
    }

    void addPreUpdateHandler(std::function<void()> callback) {
      preUpdateHandlers_.push_back(callback);
    }

  private:
    std::unique_ptr<ComponentManager> componentManager_;
    std::unique_ptr<EntityManager> entityManager_;
    std::unique_ptr<SystemManager> systemManager_;
    std::shared_ptr<InputHandler> inputHandler_;
    std::vector<std::function<void()>> postUpdateHandlers_;
    std::vector<std::function<void()>> preUpdateHandlers_;
};

#endif // CONTEXT_H
