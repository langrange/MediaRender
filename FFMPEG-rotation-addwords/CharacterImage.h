//
// Created by yfei on 17-8-24.
//

#ifndef FFMPEG_ADDCHARS_CHARACTERIMAGE_H
#define FFMPEG_ADDCHARS_CHARACTERIMAGE_H

#include "picture.h"
#include "json/json.h"

class CharacterImage {
public:

    Json::Value root ;
    frameData char_image ;
    int id ;
    int width ;
    int height ;
    int align ;
    int font_size ;
    char*  textfomat ;  /* first argument     */
    const  char* in_text ;
    void CharacterImageGet() ;

};


#endif //FFMPEG_ADDCHARS_CHARACTERIMAGE_H
