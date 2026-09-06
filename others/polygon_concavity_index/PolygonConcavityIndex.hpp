#include <vector>

struct Point2D {
  int x;
  int y;
};

class PolygonConcavityIndex {
 public:
  int solution(std::vector<Point2D> &A);
 private:
  bool AreOnSameXorY(const Point2D& p1, const Point2D& p2, const Point2D& p3);
};