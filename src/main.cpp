/// Entry point.
///
/// @file

#include <fstream>
#include <iostream>
#include <optional>

#include <boost/program_options.hpp>
extern "C" {
#include <libavformat/avformat.h>
}

#include "medianizer/frame.hpp"
#include "medianizer/io.hpp"

//==========================================================================

struct Config
{
  std::string video_file{};
  std::string csv_file{};
  int grid_width{};
  int grid_height{};
};

//==========================================================================

std::optional<Config> parse_args(int argc, char *argv[])
{
  Config config{};

  namespace po = boost::program_options;

  po::options_description desc("Allowed options");
  desc.add_options()
    ("help", "produce help message")
    (",i", po::value<std::string>(&config.video_file)->required(), "input video file name")
    (",o", po::value<std::string>(&config.csv_file)->required(), "output CSV file name")
    (",w", po::value<int>(&config.grid_width)->required(), "grid width")
    (",h", po::value<int>(&config.grid_height)->required(), "grid height")
    ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  if (vm.count("help"))
  {
    std::cout << desc << '\n';
    return {};
  }
  po::notify(vm);

  return config;
}

//--------------------------------------------------------------------------

int main(int argc, char *argv[])
{
  av_register_all();

  try
  {
    const auto config = parse_args(argc, argv);
    if (config == std::nullopt)
    {
      return 1;
    }

    std::ofstream csv{config->csv_file};
    csv.exceptions(~std::ofstream::goodbit);

    medianizer::process_file(config->video_file, [&](const auto& frame) {
      std::cout << frame.timestamp << '\n';
      csv << frame.timestamp;
      medianizer::frame_medians(frame, config->grid_width, config->grid_height,
                                [&](const int median) { csv << ';' << median; });
      csv << std::endl;
    });
  }
  catch (const std::exception& exc)
  {
    std::cerr << "failed: " << exc.what() << '\n';
  }

  return 0;
}
