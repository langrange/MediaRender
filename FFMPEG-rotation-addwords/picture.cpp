//
// Created by yfei on 17-8-16.
//

#include "picture.h"

#include <cstring>

#include <fstream>
#include <sstream>

picture &picture::operator+(picture & picture1){

    int index = size;
    for(int i=0;i<index;i++){
        ptr[i] = ptr[i] + picture1.ptr[i];
    }
    return *this ;
}

picture::picture(int w, int h):width(w),height(h){

    size = width*height*3;
    ptr = (uint8_t *) malloc(sizeof(uint8_t) *size);
    memset(ptr,0,size) ;

}

picture picture::_Bias(int x_delta) {

    picture temp_picture(this->width,this->height) ;

    int index = 0;
    for(int i=0;i<this->height;i++)
        for(int j=0;j<this->width;j++)
        {
          //  int index =  i*this->width + j- x_delta ;
            if(j-x_delta>=0) {
                temp_picture.ptr[(index + x_delta)*3] = this->ptr[index*3];
                temp_picture.ptr[(index + x_delta)*3+1] = this->ptr[index*3+1];
                temp_picture.ptr[(index + x_delta)*3+2] = this->ptr[index*3+2];
                ++index;
            }
        }
    return temp_picture;
}

picture picture::operator=(picture &picture1) {

    this->width =  picture1.width;
    this->height =  picture1.height ;
    this->ptr =  picture1.ptr ;
    return *this;
}

