#include "Camera.h"

bool operator == (const Camera& c1, const Camera& c2) {
  return c1.position == c2.position && c1.front == c2.front;
}

bool operator != (const Camera& c1, const Camera& c2) {
  return !(c1 == c2);
}
