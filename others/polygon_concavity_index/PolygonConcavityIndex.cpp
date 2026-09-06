#include "PolygonConcavityIndex.hpp"

int PolygonConcavityIndex::solution(std::vector<Point2D>& A) {
  auto begin = A.begin();
  size_t index = 2;
  for (auto it = A.begin() + 1; (it + 1) != A.end(); it++) {
    auto curr = it;
    if (AreOnSameXorY(*(it - 1), *it, *(it+1))) {
      continue;
    } else {
      if ((it-1)->x)
    }
  }
  return -1;
}

bool PolygonConcavityIndex::AreOnSameXorY(const Point2D& p1, const Point2D& p2, const Point2D& p3) {
  return (p1.x == p2.x && p1.x == p3.x) || (p1.y == p2.y && p1.y == p3.y);
}