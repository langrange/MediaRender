//
// Created by yfei on 17-9-6.
//

#ifndef FFMPEG_ROTATION_ADDWORDS_MATRIXCOMPUTE_H
#define FFMPEG_ROTATION_ADDWORDS_MATRIXCOMPUTE_H

#include <iostream>
int rinv(double *a,int n) ;
void trmul(double *a,double *b, double m,int n,int k,double *c) ;
char* file_Full_Path(std::string str);


#endif //FFMPEG_ROTATION_ADDWORDS_MATRIXCOMPUTE_H
