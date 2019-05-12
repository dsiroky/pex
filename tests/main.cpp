#include <gtest/gtest.h>

#include "medianizer/frame.hpp"

//==========================================================================

int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

//==========================================================================

TEST(Grid, Seams)
{
  {
    const auto seams = medianizer::grid_seams(9, 1);
    ASSERT_EQ(seams.size(), 1u);
    EXPECT_EQ(seams[0].first, 0);
    EXPECT_EQ(seams[0].last, 8);
  }
  {
    const auto seams = medianizer::grid_seams(9, 2);
    ASSERT_EQ(seams.size(), 2u);
    EXPECT_EQ(seams[0].first, 0);
    EXPECT_EQ(seams[0].last, 3);
    EXPECT_EQ(seams[1].first, 4);
    EXPECT_EQ(seams[1].last, 8);
  }
  {
    const auto seams = medianizer::grid_seams(9, 3);
    ASSERT_EQ(seams.size(), 3u);
    EXPECT_EQ(seams[0].first, 0);
    EXPECT_EQ(seams[0].last, 2);
    EXPECT_EQ(seams[1].first, 3);
    EXPECT_EQ(seams[1].last, 5);
    EXPECT_EQ(seams[2].first, 6);
    EXPECT_EQ(seams[2].last, 8);
  }
  {
    const auto seams = medianizer::grid_seams(9, 9);
    ASSERT_EQ(seams.size(), 9u);
    EXPECT_EQ(seams[0].first, 0);
    EXPECT_EQ(seams[0].last, 0);
    EXPECT_EQ(seams[1].first, 1);
    EXPECT_EQ(seams[1].last, 1);
    EXPECT_EQ(seams[2].first, 2);
    EXPECT_EQ(seams[2].last, 2);
    EXPECT_EQ(seams[3].first, 3);
    EXPECT_EQ(seams[3].last, 3);
    EXPECT_EQ(seams[4].first, 4);
    EXPECT_EQ(seams[4].last, 4);
    EXPECT_EQ(seams[5].first, 5);
    EXPECT_EQ(seams[5].last, 5);
    EXPECT_EQ(seams[6].first, 6);
    EXPECT_EQ(seams[6].last, 6);
    EXPECT_EQ(seams[7].first, 7);
    EXPECT_EQ(seams[7].last, 7);
    EXPECT_EQ(seams[8].first, 8);
    EXPECT_EQ(seams[8].last, 8);
  }
}

//==========================================================================

TEST(Calculate, Median)
{
  const auto data = [](std::vector<uint8_t>&& c) {
    return gsl::span<uint8_t>{c.data(), static_cast<ssize_t>(c.size())};
  };

  EXPECT_EQ(0u, medianizer::median(data({0})));
  EXPECT_EQ(255u, medianizer::median(data({255})));
  EXPECT_EQ(9, medianizer::median(data({3, 9})));
  EXPECT_EQ(9, medianizer::median(data({9, 3})));
  EXPECT_EQ(4, medianizer::median(data({1, 4, 10})));
  EXPECT_EQ(4, medianizer::median(data({4, 10, 1})));
  EXPECT_EQ(4, medianizer::median(data({10, 4, 1})));
  EXPECT_EQ(4, medianizer::median(data({10, 1, 4})));
  EXPECT_EQ(8, medianizer::median(data({8, 10, 1, 4})));
}
