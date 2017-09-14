//
// Created by yfei on 17-8-2.
//

#ifndef FFMPEG_YUV2H264_FRADATA_H
#define FFMPEG_YUV2H264_FRADATA_H

#include <iostream>
#include <vector>

typedef struct {
    uint8_t r=0;
    uint8_t g=0;
    uint8_t b=0;
}RGB_;



typedef struct {
    //arrays for storing the R,G,B values
    std::vector<uint8_t > r;
    std::vector<uint8_t > g;
    std::vector<uint8_t > b;
}image;

typedef struct {
    //arrays for storing the R,G,B values
    uint8_t * ptr_rgb ;
    int length ;
}Image;

typedef struct {
    Image imageId ;
    int width ;
    int height ;
    int animation ;
}frameData;

typedef struct {
    char* video_name ;
}BackMask;

class picture {

public:
    //arrays for storing the R,G,B values

    int size ;
    int width ;
    int height ;
    uint8_t * ptr ;
    picture &operator+(picture & picture1) ;
    picture operator=(picture & picture1) ;
    picture _Bias(int x_delta) ;
    picture (int w,int h) ;
   // void write(const std::string &fname);
   picture();
};


#endif //FFMPEG_YUV2H264_FRADATA_H
