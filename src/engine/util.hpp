#ifndef UTIL_HPP
#define UTIL_HPP

#include <boost/math/constants/constants.hpp>

namespace util {
  std::string uuid();

  double random( double min, double max );

  const double pi = boost::math::constants::pi<double>();
}

#endif
