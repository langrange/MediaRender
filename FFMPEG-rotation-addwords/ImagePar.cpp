//
// Created by yfei on 17-8-17.
//
#include "ImagePar.h"

#include <fstream>
#include <sstream>
#include <algorithm>

#include "ReadImage.h"

void ImagePar::GetJsonData() {

    std::ifstream in(FrameFormatfile_path, std::ios::binary);
    if (!in.is_open()) {
        std::cout << "Error opening file\n";
    }
    reader.parse(in, root) ;
    in.close();

}

void ImagePar::readimagedata() {

    GetJsonData() ;
    ReadImage imagecontent ;
    imagecontent.imagepath = imagepath ;
    imagecontent.readimagedata();
    imageId = imagecontent.imageId ;
    width = imagecontent.width ;
    height = imagecontent.height ;
}



