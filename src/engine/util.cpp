#include "util.h"

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <random>

std::string util::uuid() {
  return boost::uuids::to_string(boost::uuids::random_generator()());
}

double util::random( double min, double max ) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis( min, max );
  return dis(gen);
}
