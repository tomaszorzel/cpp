#include <gtest/gtest.h>

#include "StringFormatter.hpp"

class StringFormatterTest : public ::testing::Test {};

TEST_F(StringFormatterTest, OneColumnOfWidth2) {
  StringFormatter formatter("SS-4567 3456-6674 SS4");
  formatter.Format();
  EXPECT_EQ("SS4 567 345 666 74S S4;", formatter.GetFormatted());
}

TEST_F(StringFormatterTest, AllColumnsOfWidth3) {
  StringFormatter formatter("SS-4567 3456-6674 SS45");
  formatter.Format();
  EXPECT_EQ("SS4 567 345 666 74S S45;", formatter.GetFormatted());
}

TEST_F(StringFormatterTest, TwoColumnOfWidth2) {
  StringFormatter formatter("SS-4567 3456-6674 SS45A");
  formatter.Format();
  EXPECT_EQ("SS4 567 345 666 74S S4 5A;", formatter.GetFormatted());
}

TEST_F(StringFormatterTest, Only2Characters) {
  StringFormatter formatter("Sa");
  formatter.Format();
  EXPECT_EQ("Sa;", formatter.GetFormatted());
}
TEST_F(StringFormatterTest, Only2CharactersWithTrailingSpace) {
  StringFormatter formatter("Sa ");
  formatter.Format();
  EXPECT_EQ("Sa;", formatter.GetFormatted());
}

TEST_F(StringFormatterTest, Only2CharactersWithTrailingDash) {
  StringFormatter formatter("S5-");
  formatter.Format();
  EXPECT_EQ("S5;", formatter.GetFormatted());
}