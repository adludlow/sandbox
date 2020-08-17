#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace util {
  std::string uuid() {
    return boost::uuids::to_string(boost::uuids::random_generator()());
  }
}
