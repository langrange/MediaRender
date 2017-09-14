//
// Created by yfei on 17-8-11.
//

#ifndef FFMPEG_CREATMP4_GETPERSPECTIVEMATRIX_H
#define FFMPEG_CREATMP4_GETPERSPECTIVEMATRIX_H

class GetPerspectiveMatrix {
public:
    double x[4]={0,0,0,0};
    double y[4]={0,0,0,0};
    double u[4]={0,0,0,0};
    double v[4]={0,0,0,0};

    void PerspectiveMatrix(); //@return PerspectiveMatrix
    double Juzhen[3][3] ;  // the goal:PerspectiveMatrix

private:
    double Matr[8][8];   // 8x8线性方程组系数矩阵
};

#endif //FFMPEG_CREATMP4_GETPERSPECTIVEMATRIX_H
