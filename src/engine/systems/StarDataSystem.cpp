#include <functional>

#include "StarDataSystem.h"
#include "../components/Camera.h"

StarDataSystem::StarDataSystem() {
}

StarDataSystem::~StarDataSystem() {
  if (db != nullptr) {
    sqlite3_close(db);
    db = nullptr;
  }
}

void StarDataSystem::init(const std::string& databaseFile) {
  int rc;
  rc = sqlite3_open(
    databaseFile.c_str(),
    &this->db
  );
  if (rc) {
    fprintf(stderr, "Can't open star database: %s\n", sqlite3_errmsg(this->db));
  }
  else {
    fprintf(stdout, "Star database opened successfully\n");
  }
}

std::set<Entity> StarDataSystem::entities() {
  return entities_;
}

void StarDataSystem::addEntity(Entity entity) {

}

void StarDataSystem::deleteEntity(Entity entity) {

}

int StarDataSystem::queryCallback(void *notRequired, int numCol, char** rowData, char** colNames) {
  for (int i = 0; i < numCol; i++) {
    fprintf(stdout, "%s: %s\n", colNames[i], rowData[i] ? rowData[i] : "NULL");
  }
  printf("\n");
  return 0;
}

void StarDataSystem::update(float dt) {
  printf("StarDataSystem::update()\n");
  char* err = 0;

  for (auto entity: entities_) {
    auto& camera = ctx->getComponent<Camera>(entity);
    char* sql = "select * from star where x >= -5 and x <= 5 and y >= -5 and y <= 5 and z >= -5 and z <= 5";
    int result = sqlite3_exec(this->db, sql, &StarDataSystem::queryCallback, (void*)"Callback Called", &err);
  }
}
