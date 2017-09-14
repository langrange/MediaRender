//
// Created by yfei on 17-8-11.
//
#include "FrameComposed.h"

#include <math.h>
#include <string.h>

#include "MatrixCompute.h"
#include "inRect.h"
//#include "Distance.h"
#include "GetPerspectiveMatrix.h"
#include "picture.h"

#define  BLACK 0

void ImageConvert(frameData* inputImage,uint8_t *&outImage,double translate[2], double angle ,double scale ,
                  double out_size[2],double transparency,bool istext){

    int width = inputImage->width ;
    int height = inputImage->height ;

    int outwidth = out_size[0] ;
    int outheight = out_size[1] ;

    int  x_cen_init = width>>1  ;   //find the centroid of the original image
    int  y_cen_init = height>>1  ;

    /// 四个原图的顶点坐标变化为齐坐标
    double temp_LeftDown[3][1] = {{0 - x_cen_init},
                                  {0 - y_cen_init},
                                  {1}};
    double temp_LeftUp[3][1] ={{0 - x_cen_init},
                               {height - y_cen_init},
                               {1}};
    double temp_RightDown[3][1] ={{width - x_cen_init},
                                  {0 - y_cen_init},
                                  {1}};

    double temp_RightUp[3][1] = {{width - x_cen_init},
                                 {height - y_cen_init},
                                 {1}};

    double scale1 , scale2;
    scale1 = outwidth ;
    scale1 = scale1/width ;
    scale2 = outheight ;
    scale2 = scale2/height ;

    double theta =  angle * M_PI / 180 ;

    double SinTheta = sin(theta) ;
    double CosTheta = cos(theta) ;

    double MatPro[3][3] ;   /// 最终动作矩阵

    double MatTranslate[3][3] = {{1, 0, translate[0]},   ///平移矩阵
                                 {0, 1, translate[1]},
                                 {0, 0, 1}};

    double MatShap[3][3] ={{scale, 0, 0},  ///缩放矩阵
                           {0, scale, 0},
                           {0, 0, 1}};

    double Matangle[3][3] = {{CosTheta,-SinTheta,0}, /// 旋转矩阵
                             {SinTheta,CosTheta,0},
                             {0,0,1}};

    double MatStandard[3][3]={{scale1,0,0}, /// 标准输出缩放矩阵
                              {0,scale2,0},
                              {0,0,1}};

    if(!istext)  {   /// is_word
        /// 各种动作相乘为最后的动作矩阵
        double matrix_temp[3][3];
        double matrix_temp2[3][3];
        trmul((double *)MatShap,(double *)MatStandard,3,3,3,(double *)matrix_temp);
        trmul((double *)Matangle,(double *)matrix_temp,3,3,3,(double *)matrix_temp2);
        trmul((double *)MatTranslate,(double *)matrix_temp2,3,3,3,(double *)MatPro);

    }
    else{  /// is_image
        /// 各种动作相乘为最后的动作矩阵
        double matrix_zhongjian[3][3];
        trmul((double *)Matangle,(double *)MatShap,3,3,3,(double *)matrix_zhongjian);
        trmul((double *)MatTranslate,(double *)matrix_zhongjian,3,3,3,(double *)MatPro);

    }

    /** define the boundary of the new image ***/
    double LeftDown[3][1],LeftUp[3][1],RightDown[3][1],RightUp[3][1];
    trmul((double*)MatPro,(double *)temp_LeftDown,3,3,1,(double *)LeftDown);
    trmul((double*)MatPro,(double *)temp_LeftUp,3,3,1,(double *)LeftUp);
    trmul((double*)MatPro,(double *)temp_RightDown,3,3,1,(double *)RightDown);
    trmul((double*)MatPro,(double *)temp_RightUp,3,3,1,(double *)RightUp);

    double x[4] ={RightDown[0][0] / RightDown[2][0],RightUp[0][0] / RightUp[2][0],LeftUp[0][0] / LeftUp[2][0],LeftDown[0][0] / LeftDown[2][0]};
    double y[4] ={RightDown[1][0] / RightDown[2][0],RightUp[1][0] / RightUp[2][0],LeftUp[1][0] / LeftUp[2][0],LeftDown[1][0] / LeftDown[2][0]};

    ///转换后图片的四个顶点坐标
    Point Point_LD(x[0], y[0]);    //define the four boundary point of new image
    Point Point_LU(x[1], y[1]);
    Point Point_RU(x[2], y[2]);
    Point Point_RD(x[3], y[3]);

    double xmin = std::min(x[3],std::min(x[2],std::min(x[0],x[1])));
    double xmax = std::max(x[3],std::max(x[2],std::max(x[0],x[1])));
    double ymax = std::max(y[3],std::max(y[2],std::max(y[0],y[1])));
    double ymin = std::min(y[3],std::min(y[2],std::min(y[0],y[1])));

    xmin = int(xmin>0?xmin:0);
    xmax = int(xmax<outwidth?xmax:outwidth);
    ymin = int(ymin>0?ymin:0);
    ymax = int(ymax<outheight?ymax:outheight);

    long  indx = xmin+ymin*outwidth;

    rinv((double *)MatPro,3) ;  /// 原始变换矩阵MatPro的逆矩阵
    double Point_Original[3][1] ;
    /// the pixel is given the RGB value


    for (int Col = ymin; Col <ymax; Col++) {
        for (int Row = xmin; Row <xmax; Row++) {

            //indx = Col * outwidth + Row;
            Point Point_test(Row, Col);
            if( ((Point_LU.x - Point_LD.x) * (Point_test.y - Point_LD.y) -(Point_test.x - Point_LD.x) * (Point_LU.y - Point_LD.y))
                * ((Point_RD.x - Point_RU.x) * (Point_test.y - Point_RU.y) -(Point_test.x - Point_RU.x) * (Point_RD.y - Point_RU.y))>0&&
                ((Point_RU.x - Point_LU.x) * (Point_test.y - Point_LU.y) -(Point_test.x - Point_LU.x) * (Point_RU.y - Point_LU.y))*
                ((Point_LD.x - Point_RD.x) * (Point_test.y - Point_RD.y) -(Point_test.x - Point_RD.x) * (Point_LD.y - Point_RD.y))>0){

                // judge if the current point is in the new image area (if YES then)
                // if (IsPointInMatrix(Point_LD, Point_LU, Point_RU, Point_RD, Point_test)) {

                double M_point[3][1] = {{Row},
                                        {Col},
                                        {1}};
                ///  向后映射到原始图像点
                trmul((double*)MatPro,(double*)M_point,3,3,1,(double*)Point_Original);

                // 采用双线性插值(已取消)
                double x_Orignal = Point_Original[0][0]/ Point_Original[2][0] + x_cen_init;
                int tem = int(x_Orignal) ;
                int x_xiao = tem>0?tem:0;

                double y_Orignal = Point_Original[1][0] / Point_Original[2][0] + y_cen_init;
                int ytem = int(y_Orignal) ;
                int y_xiao = ytem>0?ytem:0;

                memcpy(outImage+indx*3,inputImage->imageId.ptr_rgb+((y_xiao) * width + x_xiao)*3,3);

            }
            ++indx ;
            if(Row ==xmax-1){
                indx = indx+outwidth+xmin-xmax ;
            }

        }
    }

}

void ImageConvert_Pespective(frameData* inputImage,uint8_t *&outImage,double translate[2],double out_size[2],double *PerspectiveMatrix,double transparency){

    int width = inputImage->width ;
    int height = inputImage->height ;

    int outwidth = out_size[0] ;
    int outheight = out_size[1] ;

    double x_cen_init = round( width / 2);   //find the centroid of the original image
    double y_cen_init = round( height / 2);

    /// 原图四个顶点坐标
    Point leftdown_init(0 - x_cen_init, 0 - y_cen_init);
    Point leftup_init(0 - x_cen_init, height - y_cen_init);
    Point rightdown_init(width - x_cen_init, 0 - y_cen_init);
    Point rightup_init(width - x_cen_init, height - y_cen_init);

    /// 四个原图的顶点坐标变化为齐坐标
    double temp_LeftDown[3][1] = {{leftdown_init.x},
                                  {leftdown_init.y},
                                  {1}};
    double temp_LeftUp[3][1] ={{leftup_init.x},
                               {leftup_init.y},
                               {1}};
    double temp_RightDown[3][1] ={{rightdown_init.x},
                                  {rightdown_init.y},
                                  {1}};
    double temp_RightUp[3][1] = {{rightup_init.x},
                                 {rightup_init.y},
                                 {1}};

    double scale1 , scale2;   ///横纵坐标缩放比
    scale1 = outwidth ;
    scale1 = scale1/width ;
    scale2 = outheight ;
    scale2 = scale2/height ;

    double MatPro[3][3] ;   ///最终变化矩阵（各种动作的合成）

    double matrix_temp2[3][3];

    double MatTranslate[3][3] = {{1, 0, translate[0]},   ///平移矩阵
                                 {0, 1, translate[1]},
                                 {0, 0, 1}};

    double MatStandard[3][3]={{scale1,0,0},   ///标准输出缩放矩阵
                              {0,scale2,0},
                              {0,0,1}};

    /// 各种动作相乘为最后的动作矩阵
    trmul((double *)MatTranslate,(double *)MatStandard,3,3,3,(double *)matrix_temp2);
    trmul(PerspectiveMatrix,(double *)matrix_temp2,3,3,3,(double *)MatPro);

    /// define the boundary of the new image
    double LeftDown[3][1],LeftUp[3][1],RightDown[3][1],RightUp[3][1];
    trmul((double*)MatPro,(double *)temp_LeftDown,3,3,1,(double *)LeftDown);
    trmul((double*)MatPro,(double *)temp_LeftUp,3,3,1,(double *)LeftUp);
    trmul((double*)MatPro,(double *)temp_RightDown,3,3,1,(double *)RightDown);
    trmul((double*)MatPro,(double *)temp_RightUp,3,3,1,(double *)RightUp);


    double x[4] ={RightDown[0][0] / RightDown[2][0],RightUp[0][0] / RightUp[2][0],LeftUp[0][0] / LeftUp[2][0],LeftDown[0][0] / LeftDown[2][0]};
    double y[4] ={RightDown[1][0] / RightDown[2][0],RightUp[1][0] / RightUp[2][0],LeftUp[1][0] / LeftUp[2][0],LeftDown[1][0] / LeftDown[2][0]};

    ///转换后图片的四个顶点坐标
    Point Point_LD(x[0], y[0]);    //define the four boundary point of new image
    Point Point_LU(x[1], y[1]);
    Point Point_RU(x[2], y[2]);
    Point Point_RD(x[3], y[3]);

    double xmin = std::min(x[3],std::min(x[2],std::min(x[0],x[1])));
    double xmax = std::max(x[3],std::max(x[2],std::max(x[0],x[1])));
    double ymax = std::max(y[3],std::max(y[2],std::max(y[0],y[1])));
    double ymin = std::min(y[3],std::min(y[2],std::min(y[0],y[1])));

    xmin = int(xmin>0?xmin:0);
    xmax = int(xmax<outwidth?xmax:outwidth);
    ymin = int(ymin>0?ymin:0);
    ymax = int(ymax<outheight?ymax:outheight);

    long  indx = xmin+ymin*outwidth;

    rinv((double *)MatPro,3) ;  /// 原始变换矩阵MatPro的逆矩阵
    double Point_Original[3][1] ;
    /// the pixel is given the RGB value

    for (int Col = ymin; Col <ymax; Col++) {
        for (int Row = xmin; Row <xmax; Row++) {

            //indx = Col * outwidth + Row;
            Point Point_test(Row, Col);
            if( ((Point_LU.x - Point_LD.x) * (Point_test.y - Point_LD.y) -(Point_test.x - Point_LD.x) * (Point_LU.y - Point_LD.y))
                * ((Point_RD.x - Point_RU.x) * (Point_test.y - Point_RU.y) -(Point_test.x - Point_RU.x) * (Point_RD.y - Point_RU.y))>0&&
                ((Point_RU.x - Point_LU.x) * (Point_test.y - Point_LU.y) -(Point_test.x - Point_LU.x) * (Point_RU.y - Point_LU.y))*
                ((Point_LD.x - Point_RD.x) * (Point_test.y - Point_RD.y) -(Point_test.x - Point_RD.x) * (Point_LD.y - Point_RD.y))>0){

                // judge if the current point is in the new image area (if YES then)
                // if (IsPointInMatrix(Point_LD, Point_LU, Point_RU, Point_RD, Point_test)) {

                double M_point[3][1] = {{Row},
                                        {Col},
                                        {1}};
                ///  向后映射到原始图像点
                trmul((double*)MatPro,(double*)M_point,3,3,1,(double*)Point_Original);

                // 采用双线性插值(已取消)
                double x_Orignal = Point_Original[0][0]/ Point_Original[2][0] + x_cen_init;
                int tem = int(x_Orignal) ;
                int x_xiao = tem>0?tem:0;

                double y_Orignal = Point_Original[1][0] / Point_Original[2][0] + y_cen_init;
                int ytem = int(y_Orignal) ;
                int y_xiao = ytem>0?ytem:0;

                memcpy(outImage+indx*3,inputImage->imageId.ptr_rgb+((y_xiao) * width + x_xiao)*3,3);

            }
            ++indx ;
            if(Row ==xmax-1){
                indx = indx+outwidth+xmin-xmax ;
            }
        }
    }
}


void FrameComposed::addimage() {

    bool istext = false ;

    double out_size[2] = {width,height};

    numFrame =  videoframebg.size();
    videoframegoal.resize(numFrame) ;

    int ImageSize = width * height * 3 ;

    clock_t start0,finish0;
    double totaltime0;
    start0=clock();
    for(int j=0;j<numFrame;j++){

        uint8_t * imagesum ;
        imagesum = (uint8_t*)malloc(ImageSize) ;
        memset(imagesum,0,ImageSize);

        for(int i=0;i<image_action_Tag.size();i++){
            if(image_action_Tag[i][j]["switch"].asBool()) {
                if(2 !=imageinput[i].animation) {
                    // 透视之外的动作
                    double scale = image_action_Tag[i][j]["scale"].asDouble() / 100;
                    double transparency;

                    if (0  == image_action_Tag[i][j]["alpha"].size())
                        transparency = 1;
                    else {
                        transparency = image_action_Tag[i][j]["alpha"].asDouble() / 100;
                    }

                    double translate[2] = {image_action_Tag[i][j]["x"].asDouble(),
                                           image_action_Tag[i][j]["y"].asDouble()};

                    double angle = image_action_Tag[i][j]["angle"].asDouble();

                    istext = false;
                    if(scale) ImageConvert(& imageinput[i],imagesum,translate, angle, scale, out_size, transparency, istext);

                    //   free(imageinput[i].imageId.ptr_rgb);
                    //   imageinput[i].imageId.ptr_rgb = NULL ;

                }else{
                    // is_透视动作
                    GetPerspectiveMatrix perspectiveDoing;

                    perspectiveDoing.x[0] = 0;        //透视变换四个顶点一一对应
                    perspectiveDoing.x[1] = width-1;
                    perspectiveDoing.x[2] = 0;
                    perspectiveDoing.x[3] = width-1;

                    perspectiveDoing.y[0] = 0;
                    perspectiveDoing.y[1] = 0;
                    perspectiveDoing.y[2] = height-1;
                    perspectiveDoing.y[3] = height-1;

                    perspectiveDoing.u[0] = image_action_Tag[i][j]["tl_x"].asDouble();
                    perspectiveDoing.u[1] = image_action_Tag[i][j]["tr_x"].asDouble();
                    perspectiveDoing.u[2] = image_action_Tag[i][j]["bl_x"].asDouble();
                    perspectiveDoing.u[3] = image_action_Tag[i][j]["br_x"].asDouble();

                    perspectiveDoing.v[0] = image_action_Tag[i][j]["tl_y"].asDouble();
                    perspectiveDoing.v[1] = image_action_Tag[i][j]["tr_y"].asDouble();
                    perspectiveDoing.v[2] = image_action_Tag[i][j]["bl_y"].asDouble();
                    perspectiveDoing.v[3] = image_action_Tag[i][j]["br_y"].asDouble();

                    perspectiveDoing.PerspectiveMatrix();    // 求取透视矩阵
                    double translate[2] = {width/2,height/2};  // 平移量
                    double transparency = 1 ;   // 透明度
                 //   ImageConvert_Pespective(& imageinput[i],imagesum,translate,out_size, (double *)perspectiveDoing.Juzhen,transparency);

                    //   free(imageinput[i].imageId.ptr_rgb);
                    //   imageinput[i].imageId.ptr_rgb = NULL ;
                }
                break ;
            }
        }

        /// 前景背景图片按照Mask分配合成
        long index = 0 ;
        for (int l = 0;l<height;l++)
            for(int m=0;m<width;m++){

                int r = (imagesum[index] * (255 - videoframemask[j].ptr_rgb[index])+
                        videoframebg[j].ptr_rgb[index] * videoframemask[j].ptr_rgb[index])>>8;
                //    uint8_t r = videoframebg[j].ptr_rgb[index] ;
                imagesum[index++]=r;

                int g = (imagesum[index] * (255 - videoframemask[j].ptr_rgb[index]) +
                        videoframebg[j].ptr_rgb[index] * videoframemask[j].ptr_rgb[index])>>8;
                //   uint8_t g= videoframebg[j].ptr_rgb[index];
                imagesum[index++]=g;

                int b = (imagesum[index] * (255 - videoframemask[j].ptr_rgb[index]) +
                        videoframebg[j].ptr_rgb[index] * videoframemask[j].ptr_rgb[index])>>8;
                // uint8_t b= videoframebg[j].ptr_rgb[index];
                imagesum[index++]=b;

            }

        // Add text image
        for (int i = 0; i < char_action_Tag.size(); i++) {
            uint8_t * image_jj ;
            image_jj = (uint8_t*)malloc(ImageSize) ;
            if (char_action_Tag[i][j]["switch"].asBool()) {

                memset(image_jj,0,ImageSize);

                double scale = char_action_Tag[i][j]["scale"].asDouble() / 100;
                double transparency;
                if (image_action_Tag[i][j]["alpha"].size() == 0)
                    transparency = 1;
                else {
                    transparency = image_action_Tag[i][j]["alpha"].asDouble() / 100;
                }
                double translate[2] = {char_action_Tag[i][j]["x"].asDouble(),
                                       char_action_Tag[i][j]["y"].asDouble()};
                double angle = char_action_Tag[i][j]["angle"].asDouble();

                istext = 1; // is_文字图片

                if(scale)
                {
                    ImageConvert(& charinput[i],image_jj,translate,angle,
                                 scale,out_size,transparency,istext);
                }

                //  free(charinput[i].imageId.ptr_rgb);
                //  charinput[i].imageId.ptr_rgb = NULL ;

                int size = width * height ;
                for(int i=0;i<size ;i++){
                    if(image_jj[i*3]||image_jj[i*3+1]||image_jj[i*3+2])
                        memcpy(imagesum+i*3,image_jj+i*3,3) ;
                }

                // break;
            }
            free(image_jj);
            image_jj = NULL ;
        }

        // if(j>25) break ;

        videoframegoal[j].ptr_rgb= imagesum;  // Push 处理好的每祯图像
        videoframegoal[j].length= ImageSize;  // Push 处理好的每祯图像

        char out[40];
        if(j%20==0&&j!=0) {
            sprintf(out, "第%d~%d帧花费时间为", j-20,j);
            finish0 = clock();
            totaltime0 = (double) (finish0 - start0) / CLOCKS_PER_SEC;
            start0 = finish0 ;
            std::cout << out << totaltime0 << "秒" << std::endl;
        }
    }

    for(int i=0;i<imageinput.size();i++){
        free(imageinput[i].imageId.ptr_rgb);
        imageinput[i].imageId.ptr_rgb=NULL ;
    }
    for(int i=0;i<charinput.size();i++){
        free(charinput[i].imageId.ptr_rgb);
        charinput[i].imageId.ptr_rgb=NULL ;
    }
    for(int i=0;i<numFrame;i++){
        free(videoframebg[i].ptr_rgb);
        videoframebg[i].ptr_rgb = NULL ;
        free(videoframemask[i].ptr_rgb);
        videoframemask[i].ptr_rgb = NULL ;
    }

}
