/// @copydoc frame.hpp
///
/// @file

#include <cassert>
#include <iostream>

#include "medianizer/frame.hpp"

//==========================================================================
namespace medianizer {
//==========================================================================

uint8_t median(const gsl::span<uint8_t> data)
{
  assert(data.size() > 0);
  std::nth_element(data.begin(), data.begin() + data.size() / 2, data.end());
  return data[data.size() / 2];
}

//--------------------------------------------------------------------------

std::vector<GridSeam> grid_seams(const int length, const int dimension)
{
  assert(dimension > 0);
  assert(dimension <= length);

  std::vector<GridSeam> seams(dimension);

  int offset{0};
  for (int i{0}; i < dimension; ++i)
  {
    const int cell_size{(length - offset) / (dimension - i)};
    assert(cell_size > 0);
    seams[i].first = offset;
    seams[i].last = offset + cell_size - 1;
    offset += cell_size;
  }

  return seams;
}

//--------------------------------------------------------------------------

void frame_medians(const Frame& frame, const int grid_width, const int grid_height,
                   const std::function<void(const int)> median_callback)
{
  if ((grid_width > frame.width) or (grid_height > frame.height))
  {
    throw std::invalid_argument{"grid dimensions are too big"};
  }
  if ((grid_width <= 0) or (grid_height <= 0))
  {
    throw std::invalid_argument{"grid dimensions are too small"};
  }

  // Precalculate offsets.
  const auto horizontal_seams = grid_seams(frame.width, grid_width);
  const auto vertical_seams = grid_seams(frame.height, grid_height);

  // Main goal is to minimize CPU cache misses. Use continuous memory blocks as
  // much as possible.
  // Medians will be calculated in bulks by grid rows.

  // Single grid cell in a grid row.
  struct Bin
  {
    using Values = std::vector<uint8_t>;

    GridSeam seam{};
    Values values{};
    Values::iterator values_iter{};
  };
  // Bins for a single grid row.
  std::vector<Bin> bins{};
  for (int grid_column_idx{0}; grid_column_idx < grid_width; ++grid_column_idx)
  {
    bins.push_back(Bin{});
    bins.back().seam = horizontal_seams[grid_column_idx];
    // Preallocate memory for the largest cell.
    bins.back().values.resize((frame.width / grid_width + 1)
                              * (frame.height / grid_height + 1));
  }

  auto frame_iter = frame.data.cbegin();
  for (const auto& vseam: vertical_seams)
  {
    // reset bin pointers
    for (auto& bin: bins)
    {
      bin.values_iter = bin.values.begin();
    }

    // fill bins
    for (int row{vseam.first}; row <= vseam.last; ++row)
    {
      int column_idx{0};
      for (auto& bin: bins)
      {
        const auto columns_count = bin.seam.last - column_idx + 1;
        std::copy(frame_iter, frame_iter + columns_count, bin.values_iter);
        column_idx += columns_count;
        frame_iter += columns_count;
        bin.values_iter += columns_count;
      }
    }

    // calculate medians
    for (auto& bin: bins)
    {
      const auto bin_size = std::distance(bin.values.cbegin(),
                                          Bin::Values::const_iterator{bin.values_iter});
      assert(static_cast<size_t>(bin_size) <= bin.values.size());
      median_callback(median(gsl::span<uint8_t>{bin.values.data(), bin_size}));
    }
  }

  assert(frame_iter == frame.data.cend());
}

//==========================================================================
} // namespace medianizer
