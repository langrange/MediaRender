//
// Created by yfei on 17-9-5.
//
#ifndef FFMPEG_ROTATION_ADDWORDS_READIMAGE_H
#define FFMPEG_ROTATION_ADDWORDS_READIMAGE_H

#include <iostream>
#include <vector>
#include "picture.h"

/**********************************************************************/
#define PNG_BYTES_TO_CHECK 8
#define HAVE_ALPHA 1
#define NO_ALPHA 0

class ReadImage {
public:
    const char* imagepath;
    int width , height;
    Image imageId ;
    void readimagedata();

private:
    void readjpgdata();
    void readpngdata();
};

#endif //FFMPEG_ROTATION_ADDWORDS_READIMAGE_H
