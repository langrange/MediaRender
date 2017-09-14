//
// Created by yfei on 17-8-17.
//

#ifndef FFMPEG_ROTATION_IMAGEPAR_H
#define FFMPEG_ROTATION_IMAGEPAR_H

#include <iostream>
#include <vector>

#include "json/json.h"
#include "picture.h"

/**********************************************************************/
#define PNG_BYTES_TO_CHECK 8
#define HAVE_ALPHA 1
#define NO_ALPHA 0
/******************************图片数据*********************************/
typedef  struct {

    int width, height; /* 尺寸 */
    unsigned int bit_depth;  /* 位深 */
    int flag;   /* 一个标志，表示是否有alpha通道 */
}pic_data;

class ImagePar {
public:

    const char* FrameFormatfile_path ;
    const char* imagepath ;
    int animation ;
    Json::Reader reader;   //从json文件中读取数据
    Json::Value root;
    int width ,height ;
    Image imageId ;
    void GetJsonData() ;
    void readimagedata() ;
};


#endif //FFMPEG_ROTATION_IMAGEPAR_H
