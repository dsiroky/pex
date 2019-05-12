/// Entry point.
///
/// @file

#include <iostream>

extern "C" {
#include <libavformat/avformat.h>
}

#include "medianizer/io.hpp"

//==========================================================================

int main(int argc, char *argv[])
{
  av_register_all();

  try
  {
    medianizer::process_file("judo.mp4");
  }
  catch (const std::exception& exc)
  {
    std::cerr << "failed: " << exc.what() << '\n';
  }

  return 0;
}
