/// Common types
///
/// @file

#pragma once
#ifndef TYPES_HPP__ZGR4BYMH
#define TYPES_HPP__ZGR4BYMH

#include <gsl/span>

//==========================================================================
namespace medianizer {
//==========================================================================

struct Frame
{
  double timestamp{};
  int width{};
  int height{};
  gsl::span<uint8_t> data{};
};

//==========================================================================
} // namespace medianizer

#endif /* include guard */
