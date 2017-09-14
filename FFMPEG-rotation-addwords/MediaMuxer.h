//
// Created by yfei on 17-8-11.
//

#ifndef FFMPEG_CREATMP4_MEDIAMUXER_H
#define FFMPEG_CREATMP4_MEDIAMUXER_H

#include "picture.h"

extern  "C"{
#include <libavformat/avformat.h>
};

#define  COLORSIZE 256

typedef  struct
{

    uint8_t *data1= (uint8_t *)malloc(1000000);
    uint8_t *data2= (uint8_t *)malloc(1000000);
    uint8_t *data3= (uint8_t *)malloc(1000000);

}FrameData;


typedef struct {
  uint8_t * ptr_Y;
  uint8_t * ptr_U;
  uint8_t * ptr_V;
}YuvData;

typedef struct{
    AVStream *st;
    int64_t next_pts;
    int samples_count;
    AVFrame *frame;
    AVFrame *tmp_frame;

    float t, tincr, tincr2;
    struct SwsContext *sws_ctx;
    struct SwrContext *swr_ctx;
} OutputStream;

class MediaMuxer {
public:

    std::vector <Image> videoframe_in  ;
    const char *output_MP4_file_name;
    double STREAM_FRAME_RATE[2] ;
    double STREAM_DURATION ;
    std::vector <YuvData> VideoFramedata ;
    std::vector <int16_t > temp_voice ;
    int width ;
    int height ;
    int numFrame ;
    int mediamux() ;
    MediaMuxer(const char * mp4name,std::vector <Image> &videoin,std::vector <int16_t >&voice,
               double DURATION,double FRAME_RATE[2],int wid,int hei):output_MP4_file_name(mp4name),
               videoframe_in(videoin),temp_voice(voice),STREAM_DURATION(DURATION),width(wid),height(hei){
        STREAM_FRAME_RATE[0] = FRAME_RATE[0];
        STREAM_FRAME_RATE[1] = FRAME_RATE[1];
    }


private:
    long idx=0 ;
    int encode_video = 1, encode_audio = 1;

    void image2yuv() ;
    void add_stream(OutputStream *ost, AVFormatContext *oc, AVCodec **codec, enum AVCodecID codec_id);
    void Add_audio_video_streams(OutputStream *video_st, OutputStream *audio_st, AVFormatContext *oc,
                                 AVOutputFormat *fmt, AVCodec *audio_codec, AVCodec *video_codec);
    AVFrame * get_video_frame(OutputStream *ost);
    int Write_video_frame(AVFormatContext *oc, OutputStream *ost);
    AVFrame * get_audio_frame(OutputStream *ost);
    int Write_audio_frame(AVFormatContext *oc, OutputStream *ost);

};



#endif //FFMPEG_CREATMP4_MEDIAMUXER_H
