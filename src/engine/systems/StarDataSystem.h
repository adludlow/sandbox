#ifndef STAR_DATA_SYSTEM_H
#define STAR_DATA_SYSTEM_H

#include <memory>
#include <string>
#include <sqlite3.h>

#include "../core/Context.h"
#include "../core/System.h"
#include "../components/Camera.h"

extern std::shared_ptr<Context> ctx;

class StarDataSystem : public System {
  public:
    StarDataSystem();
    ~StarDataSystem();

    void init(const std::string &databaseFile);

    virtual std::set<Entity> entities() override;
    virtual void addEntity(Entity entity) override;
    virtual void deleteEntity(Entity entity) override;
    virtual void update(float dt) override;

  private:
    std::set<Entity> entities_;
    sqlite3 *db_;
    Camera lastCameraLocation_;

    void setLastCameraLocation(const Camera& camera);
    static int queryCallback(void *notRequired, int numCol, char** rowData, char** colNames);
};

#endif //STAR_DATA_SYSTEM
