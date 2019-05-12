/// Process frame.
///
/// @file

#pragma once
#ifndef FRAME_HPP__7KA0VZKV
#define FRAME_HPP__7KA0VZKV

#include <functional>
#include <vector>

#include "medianizer/types.hpp"

//==========================================================================
namespace medianizer {
//==========================================================================

/// Column/row indexes.
struct GridSeam
{
  int first{};
  int last{};
};

//==========================================================================

/// @param data will be mutated in place
///
/// @return median of data
uint8_t median(const gsl::span<uint8_t> data);

//--------------------------------------------------------------------------

/// @param length how many frame columns/rows
/// @param dimension grid size
///
/// @return list of seams given by a frame size divided by a grid dimension
std::vector<GridSeam> grid_seams(const int length, const int dimension);

//--------------------------------------------------------------------------

/// @param frame
/// @param grid_width
/// @param grid_height
/// @param median_callback medians will be passed to this callback
///
/// @return list of medians
void frame_medians(const Frame& frame, const int grid_width, const int grid_height,
                   const std::function<void(const int)> median_callback);

//==========================================================================
} // namespace medianizer

#endif /* include guard */
