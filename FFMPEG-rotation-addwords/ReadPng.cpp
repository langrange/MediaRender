//
// Created by yfei on 17-8-17.
//
#include <stdio.h>
#include <libpng/png.h>
#include <stdlib.h>
typedef struct
{
  unsigned char r;
  unsigned char g;
  unsigned char b;
  unsigned char a;
}_rgba ;
/******************************图片数据*********************************/
typedef  struct {

    int width, height; /* 尺寸 */
    unsigned int bit_depth;  /* 位深 */
    int flag;   /* 一个标志，表示是否有alpha通道 */
    _rgba **rgba; /* 图片数组 */

}pic_data;
/**********************************************************************/
#define PNG_BYTES_TO_CHECK 8
#define HAVE_ALPHA 1
#define NO_ALPHA 0


int detect_png(char *filepath, pic_data & out)
/* 用于解码png图片 */
{
    FILE *pic_fp;
    pic_fp = fopen(filepath, "rb");
    if(pic_fp == NULL) /* 文件打开失败 */
        return -1;

    /* 初始化各种结构 */
    png_structp png_ptr;
    png_infop  info_ptr;
    char        buf[PNG_BYTES_TO_CHECK];
    int        temp;

    png_ptr  = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
    info_ptr = png_create_info_struct(png_ptr);

    setjmp(png_jmpbuf(png_ptr)); // 这句很重要

    temp = fread(buf,1,PNG_BYTES_TO_CHECK,pic_fp);

    temp = png_sig_cmp( (png_bytep)buf, (png_size_t)0, PNG_BYTES_TO_CHECK);

    /*检测是否为png文件*/
    if (temp!=0) return 1;

    rewind(pic_fp);
    /*开始读文件*/
    png_init_io(png_ptr, pic_fp);
    // 读文件了
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);

    int color_type,channels;

    /*获取宽度，高度，位深，颜色类型*/
    channels      = png_get_channels(png_ptr, info_ptr); /*获取通道数*/
    out.bit_depth = png_get_bit_depth(png_ptr, info_ptr); /* 获取位深 */
    color_type    = png_get_color_type(png_ptr, info_ptr); /*颜色类型*/

    int size, pos = 0;
    /* row_pointers里边就是rgba数据 */
    png_byte ** row_pointers;
    row_pointers = png_get_rows(png_ptr, info_ptr);
    out.width = png_get_image_width(png_ptr, info_ptr);
    out.height = png_get_image_height(png_ptr, info_ptr);

       if(channels == 4 || color_type == PNG_COLOR_TYPE_RGB_ALPHA)
    {/*如果是RGB+alpha通道，或者RGB+其它字节*/
        out.rgba = new _rgba*[out.height] ;

        out.flag = HAVE_ALPHA;    /* 标记 */

        for(int i = 0; i < out.height; i++)
        {
            out.rgba[i] = new _rgba[out.width] ;
            for(int j = 0; j < out.width;j++)
            {/* 一个字节一个字节的赋值 */
                out.rgba[i][j].r = row_pointers[i][j*4]; // red
                out.rgba[i][j].g = row_pointers[i][j*4+1]; // green
                out.rgba[i][j].b = row_pointers[i][j*4+2];  // blue
                out.rgba[i][j].a = row_pointers[i][j*4+3]; // alpha
                ++pos;

            }
        }
    }
    else if(channels == 3 || color_type == PNG_COLOR_TYPE_RGB)
    {/* 如果是RGB通道 */
        out.rgba = new _rgba*[out.height] ;

        out.flag = HAVE_ALPHA;    /* 标记 */


        temp = (3 * out.width);/* 每行有3 * out->width个字节 */
        for(int i = 0; i < out.height; i++)
        {
            out.rgba[i] = new _rgba[out.width] ;

            for(int j = 0; j <out.width;j++)
            {/* 一个字节一个字节的赋值 */
                out.rgba[i][j].r = row_pointers[i][j*4]; // red
                out.rgba[i][j].g = row_pointers[i][j*4+1]; // green
                out.rgba[i][j].b = row_pointers[i][j*4+2];  // blue
               // out.rgba[i][j].a = row_pointers[i][j+3]; // alpha
                ++pos;

            }
        }
    }
    else return 1;

    /* 撤销数据占用的内存 */
    png_destroy_read_struct(&png_ptr, &info_ptr, 0);

    return 0;
}
int main()
{
    char * filepath = "../Media/decoration1.png";

    pic_data data_rgb ;

    detect_png(filepath,data_rgb) ;

    for(int i=0;i<data_rgb.height;i++)
        delete[] data_rgb.rgba[i] ;
    delete[] data_rgb.rgba;

    return 0;
}
