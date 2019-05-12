/// @copydoc io.hpp
///
/// @file

#include <stdexcept>
#include <iostream>

#include <boost/scope_exit.hpp>
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}

#include "medianizer/memory.hpp"
#include "medianizer/io.hpp"

//==========================================================================
namespace medianizer {
//==========================================================================

static int check_av_call(const std::string& prefix, const int ret)
{
  if (ret < 0)
  {
    throw std::runtime_error{prefix + " failed: " + std::to_string(ret)};
  }
  return ret;
}

//--------------------------------------------------------------------------

void process_file(const std::string& infile)
{
  // This rubbish code is copied (and altered) from
  // https://gist.github.com/yohhoy/f0444d3fc47f2bb2d0e2.
  // It contains deprecated calls, goto, raw pointers, ...
  // I didn't want to spend a week on learning ffmpeg.

  // Lines directly related to the task are marked with "TTTTTTTTTTTT".

  //--------------------------------

  int ret;

  // open input file context
  AVFormatContext* inctx = nullptr;
  check_av_call("avformat_open_input",
                avformat_open_input(&inctx, infile.c_str(), nullptr, nullptr));
  // retrive input stream information
  check_av_call("avformat_find_stream_info", avformat_find_stream_info(inctx, nullptr));

  // find primary video stream
  AVCodec* vcodec = nullptr;
  ret = check_av_call("av_find_best_stream",
                      av_find_best_stream(inctx, AVMEDIA_TYPE_VIDEO, -1, -1, &vcodec, 0));
  const int vstrm_idx = ret;
  AVStream* vstrm = inctx->streams[vstrm_idx];

  // open video decoder context
  check_av_call("avcodec_open2", avcodec_open2(vstrm->codec, vcodec, nullptr));

  const double time_base{av_q2d(vstrm->codec->time_base)};

  // initialize sample scaler
  const int dst_width = vstrm->codec->width;
  const int dst_height = vstrm->codec->height;
  // TTTTTTTTTTTT
  const AVPixelFormat dst_pix_fmt = AV_PIX_FMT_GRAY8;
  SwsContext* swsctx = sws_getCachedContext(
      nullptr, vstrm->codec->width, vstrm->codec->height, vstrm->codec->pix_fmt,
      dst_width, dst_height, dst_pix_fmt, SWS_BICUBIC, nullptr, nullptr, nullptr);
  if (!swsctx)
  {
    throw std::runtime_error{"fail to sws_getCachedContext"};
  }

  // allocate frame buffer for output
  AVFrame* frame = av_frame_alloc();
  std::vector<uint8_t> framebuf(avpicture_get_size(dst_pix_fmt, dst_width, dst_height));
  avpicture_fill(reinterpret_cast<AVPicture*>(frame), framebuf.data(), dst_pix_fmt,
                 dst_width, dst_height);

  // decoding loop
  AVFrame* decframe = av_frame_alloc();
  unsigned nb_frames = 0;
  bool end_of_stream = false;
  int got_pic = 0;
  AVPacket pkt;
  do
  {
    if (!end_of_stream)
    {
      // read packet from input file
      ret = av_read_frame(inctx, &pkt);
      if (ret < 0 && ret != AVERROR_EOF)
      {
        throw std::runtime_error{"fail to av_read_frame: ret=" + std::to_string(ret)};
      }
      if (ret == 0 && pkt.stream_index != vstrm_idx)
        goto next_packet;
      end_of_stream = (ret == AVERROR_EOF);
    }
    if (end_of_stream)
    {
      // null packet for bumping process
      av_init_packet(&pkt);
      pkt.data = nullptr;
      pkt.size = 0;
    }
    // decode video frame
    avcodec_decode_video2(vstrm->codec, decframe, &got_pic, &pkt);
    if (!got_pic)
    {
      goto next_packet;
    } else {
      // TTTTTTTTTTTT
      // keyframe
      if (decframe->pict_type == AV_PICTURE_TYPE_I)
      {
        // convert to grayscale
        sws_scale(swsctx, decframe->data, decframe->linesize, 0, decframe->height,
                  frame->data, frame->linesize);
        const double timestamp = (frame->pts == AV_NOPTS_VALUE) ? nb_frames * time_base
                                                                : frame->pts * time_base;
      }
    }
    ++nb_frames;
  next_packet:
    av_free_packet(&pkt);
  } while (!end_of_stream || got_pic);

  av_frame_free(&decframe);
  avcodec_close(vstrm->codec);
  avformat_close_input(&inctx);
}

//==========================================================================
} // namespace medianizer
 // namespace medianizer
