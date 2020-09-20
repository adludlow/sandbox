#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "uuid.h"

std::string util::uuid() {
  return boost::uuids::to_string(boost::uuids::random_generator()());
}
