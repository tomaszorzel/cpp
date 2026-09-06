#include <gtest/gtest.h>
#include <vector>

#include "PolygonConcavityIndex.hpp"

class PolygonConcavityIndexTest : public ::testing::Test {};

TEST_F(PolygonConcavityIndexTest, SimpleConvex) {
  std::vector<Point2D> points{{.x = -1, .y = 3}, {.x = 3, .y = 1}, {.x = 0, .y = -1}, {.x = -2, .y = 1}};
  PolygonConcavityIndex polygon_concavity_index;
  EXPECT_EQ(-1, polygon_concavity_index.solution(points));
}

TEST_F(PolygonConcavityIndexTest, SimpleConcave) {
  std::vector<Point2D> points{{.x = -1, .y = 3}, {.x = 1, .y = 2},  {.x = 3, .y = 1}, {.x = 1, .y = 1},
                              {.x = 0, .y = -1}, {.x = -2, .y = 1}, {.x = -1, .y = 2}};
  PolygonConcavityIndex polygon_concavity_index;
  EXPECT_EQ(2, polygon_concavity_index.solution(points));
}