//
// Created by yfei on 17-9-8.
//

#ifndef FFMPEG_ROTATION_ADDWORDS_FRAMESTRUCT_H
#define FFMPEG_ROTATION_ADDWORDS_FRAMESTRUCT_H

#include "ImagePar.h"

typedef struct {

    std::vector <ImagePar> ImageContent;   // read image_data
    std::vector <BackMask> BgMaskContent;  //Mask and background media
    std::vector <frameData> charinput;
    std::vector <frameData> imageinput;
    std::vector <Json::Value> char_action_Tag;
    std::vector <Json::Value> image_action_Tag;

}FrameAction;
#endif //FFMPEG_ROTATION_ADDWORDS_FRAMESTRUCT_H
