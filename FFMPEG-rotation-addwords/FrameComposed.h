//
// Created by yfei on 17-8-11.
//

#ifndef FFMPEG_CREATMP4_FRAMECOMPOSED_H
#define FFMPEG_CREATMP4_FRAMECOMPOSED_H

#include <vector>

#include "picture.h"
#include "json/json.h"
//#include "ReadImageData.h"


class FrameComposed {

public:
    int width ;
    int height ;
    int numFrame ;

    std::vector <frameData> charinput;
    std::vector <frameData> imageinput;

    std::vector<Image> videoframebg;
    std::vector<Image> videoframemask;
    std::vector<Image> videoframegoal;
    std::vector<Json::Value> image_action_Tag;
    std::vector<Json::Value> char_action_Tag;

    FrameComposed(std::vector <frameData>& charin,std::vector <frameData>& imagein,
                  std::vector<Image> & bg,std::vector<Image> & mask,
                  std::vector<Json::Value>& image_tag,std::vector<Json::Value>& char_tag,
    int wi ,int hei) :
            charinput(charin),imageinput(imagein),videoframebg(bg),videoframemask(mask),
    image_action_Tag(image_tag),char_action_Tag(char_tag),width(wi),height(hei){}

    void addimage();

};

#endif //FFMPEG_CREATMP4_FRAMECOMPOSED_H
