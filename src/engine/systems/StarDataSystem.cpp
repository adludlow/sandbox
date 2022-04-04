#include <functional>

#include "StarDataSystem.h"
#include "../components/Camera.h"

StarDataSystem::StarDataSystem() {
}

StarDataSystem::~StarDataSystem() {
  if (db_ != nullptr) {
    sqlite3_close(db_);
    db_ = nullptr;
  }
}

void StarDataSystem::init(const std::string& databaseFile) {
  int rc;
  rc = sqlite3_open(
    databaseFile.c_str(),
    &this->db_
  );
  if (rc) {
    fprintf(stderr, "Can't open star database: %s\n", sqlite3_errmsg(this->db_));
  }
  else {
    fprintf(stdout, "Star database opened successfully\n");
  }
}

std::set<Entity> StarDataSystem::entities() {
  return entities_;
}

void StarDataSystem::addEntity(Entity entity) {
  this->entities_.insert(entity);
}

void StarDataSystem::deleteEntity(Entity entity) {
  this->entities_.erase(entity);
}

int StarDataSystem::queryCallback(void *notRequired, int numCol, char** rowData, char** colNames) {
  /*for (int i = 0; i < numCol; i++) {
    fprintf(stdout, "%s: %s\n", colNames[i], rowData[i] ? rowData[i] : "NULL");
  }
  printf("\n");*/
  return 0;
}

void StarDataSystem::setLastCameraLocation(const Camera& camera) {
  this->lastCameraLocation_ = camera;
}

void StarDataSystem::update(float dt) {
  char* err = 0;

  for (auto entity: entities_) {
    auto& camera = ctx->getComponent<Camera>(entity);
    if (this->lastCameraLocation_ != camera) {
      std::cout << "Querying stars..." << std::endl;
      double minX, maxX, minY, maxY, minZ, maxZ = 0.0f;
      minX = camera.position.x - 5;
      maxX = camera.position.x + 5;
      minY = camera.position.y - 5;
      maxY = camera.position.y + 5;
      minZ = camera.position.z - 5;
      maxZ = camera.position.z + 5;

      sqlite3_stmt* selectStarsInRegion = 0;
      int rc = sqlite3_prepare_v2(
        this->db_,
        "select * from star where x >= ? and x <= ? and y >= ? and y <= ? and z >= ? and z <= ?",
        -1,
        &selectStarsInRegion,
        0
      );
      rc = sqlite3_bind_double(selectStarsInRegion, 1, minX);
      rc = sqlite3_bind_double(selectStarsInRegion, 2, maxX);
      rc = sqlite3_bind_double(selectStarsInRegion, 3, minY);
      rc = sqlite3_bind_double(selectStarsInRegion, 4, maxY);
      rc = sqlite3_bind_double(selectStarsInRegion, 5, minZ);
      rc = sqlite3_bind_double(selectStarsInRegion, 6, maxZ);

      while (sqlite3_step(selectStarsInRegion) == SQLITE_ROW) {
        
      }

      char* sql = "select * from star where x >= -5 and x <= 5 and y >= -5 and y <= 5 and z >= -5 and z <= 5";
      int result = sqlite3_exec(this->db_, sql, &StarDataSystem::queryCallback, (void*)"Callback Called", &err);

      if (err) {
        std::cout << err << std::endl;
      }

      setLastCameraLocation(camera);
      std::cout << "Stars retrieved" << std::endl;
    }
  }
}
