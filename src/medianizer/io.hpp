/// IO.
///
/// @file

#pragma once
#ifndef READER_HPP__YPRN9ZZJ
#define READER_HPP__YPRN9ZZJ

#include <string>
#include <functional>
#include <cstdint>

#include "medianizer/types.hpp"

//==========================================================================
namespace medianizer {
//==========================================================================

/// @brief Process video file.
///
/// @param infile video file name for processing
/// @param frame_callback callback receiving a frame information
void process_file(
    const std::string& infile,
    const std::function<void(const Frame&)> frame_callback);

//==========================================================================
} // namespace medianizer

#endif /* include guard */
