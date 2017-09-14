//
// Created by yfei on 17-8-23.
//

#ifndef FFMPEG_ADDCHARS_TEXT_H
#define FFMPEG_ADDCHARS_TEXT_H

#include <iostream>
#include <vector>

#include "json/json.h"
#include "picture.h"


typedef struct
{
    char * text_font_name;
    const char * font_name_path;
}font_name2font;

typedef struct{
    uint8_t r;
    uint8_t g;
    uint8_t b;
}color_size;

class text {
public:

    color_size color_value;
    const char *FrameFormatfile_path ;
    const char *AttributesFile_Path  ;
    const char *TextFile_Path  ;
    const char *in_text ;
    const char *textfomat ;
    int font_size=30 ;
    int align= 0 ;
    double x_bias=0;
    double y_bias=0;
    const char* font_name  ;
    Image imageId ;
    const char* imagepath ;
    const char* editdatapath ;

    image rgb_text ;

    Json::Reader reader;   //从json文件中读取数据
    Json::Value root;

    Json::Reader attribute_reader;   //从json文件中读取数据
    Json::Value attribute_root;

    int width ,height ;
    int image_width,image_height ;
    void CharacterImageGet() ;


private:
    void GetWordContent() ;
    void GetJsonData() ;
    void GetAttribute();
};


#endif //FFMPEG_ADDCHARS_TEXT_H
