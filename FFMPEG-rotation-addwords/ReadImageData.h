//
// Created by yfei on 17-8-15.
//

#ifndef FFMPEG_MEDIAROTATION_READIMAGEDATA_H
#define FFMPEG_MEDIAROTATION_READIMAGEDATA_H

#include <iostream>
#include <vector>

#include "picture.h"

class ReadImageData {
public:

      char *input_image_file_name ;
      int width ,height ;
      image imageData ;
      void readimagedata() ;
};


#endif //FFMPEG_MEDIAROTATION_READIMAGEDATA_H
