//
// Created by yfei on 17-9-5.
//

#include "ReadImage.h"

#include <fstream>
#include <sstream>
#include <algorithm>

#include "json/json.h"
#include "jpeglib.h"
#include "png.h"

void  ReadImage::readpngdata( )
/* 用于解码png图片 */
{
    FILE *pic_fp;
    pic_fp = fopen(imagepath, "rb");
    if(pic_fp == NULL) /* 文件打开失败 */
        std::cout << "open png file error " << std::endl ;

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
    if (temp!=0) std::cout << "open png file error " << std::endl ;

    rewind(pic_fp);
    /*开始读文件*/
    png_init_io(png_ptr, pic_fp);
    // 读文件了
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);

    int color_type,channels;

    /*获取宽度，高度，位深，颜色类型*/
    channels      = png_get_channels(png_ptr, info_ptr); /*获取通道数*/
    color_type    = png_get_color_type(png_ptr, info_ptr); /*颜色类型*/

    int size, pos = 0;
    /* row_pointers里边就是rgba数据 */
    png_byte ** row_pointers;
    row_pointers = png_get_rows(png_ptr, info_ptr);
    width= png_get_image_width(png_ptr, info_ptr);
    height= png_get_image_height(png_ptr, info_ptr);

    int ImageSize = width * height *3 ;
    uint8_t *framedata;    //allocate the memory for frame-data
    framedata= (uint8_t *) malloc(sizeof(uint8_t) *ImageSize);

    imageId.ptr_rgb = framedata ;
    imageId.length = ImageSize ;

    if(channels == 4 || color_type == PNG_COLOR_TYPE_RGB_ALPHA)
    {/*如果是RGB+alpha通道，或者RGB+其它字节*/
    int index = 0;
        for(int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {/* 一个字节一个字节的赋值 */
                framedata[index++] = row_pointers[i][j * 4]*row_pointers[i][j * 4 + 3]/255; // red
                framedata[index++] = row_pointers[i][j * 4 + 1]*row_pointers[i][j * 4 + 3]/255; // green
                framedata[index++] = row_pointers[i][j * 4 + 2]*row_pointers[i][j * 4 + 3]/255;  // blue
            }
        }

    }

    else if(channels == 3 || color_type == PNG_COLOR_TYPE_RGB)
    {/* 如果是RGB通道 */
       int index = 0 ;
        for(int i = 0; i <height; i++)
        {
            for(int j = 0; j <width;j++)
            {/* 一个字节一个字节的赋值 */
                framedata[index++]  = row_pointers[i][j*4]; // red
                framedata[index++]  = row_pointers[i][j*4+1]; // green
                framedata[index++]  = row_pointers[i][j*4+2];  // blue
            }
        }
    }
    /* 撤销数据占用的内存 */
    png_destroy_read_struct(&png_ptr, &info_ptr, 0);

}

std::string GetFilePosfix(const char* path)
{
    char* pos =(char*)strrchr(path,'.');
    if(pos)
    {
        std::string str(pos+1);
        //1.转换为小写
        //http://blog.csdn.net/infoworld/article/details/29872869
        std::transform(str.begin(),str.end(),str.begin(),::tolower);
        return str;
    }
    return std::string();
}

void ReadImage::readjpgdata(){

    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE *input_file;
    JSAMPARRAY buffer;
    int row_width;

    unsigned char *output_buffer;
    unsigned char *tmp = NULL;

    cinfo.err = jpeg_std_error(&jerr);


    if ((input_file = fopen(imagepath, "rb")) == NULL) {
        fprintf(stderr, "can't open %s\n", imagepath);
        //return -1;
    }
    // Initialization of JPEG compression objects
    jpeg_create_decompress(&cinfo);

    /* Specify data source for decompression */
    jpeg_stdio_src(&cinfo, input_file);

    /* 1.设置读取jpg文件头部，Read file header, set default decompression parameters */
    (void) jpeg_read_header(&cinfo, TRUE);

    /* 2.开始解码数据 Start decompressor */
    (void) jpeg_start_decompress(&cinfo);

    row_width = cinfo.output_width * cinfo.output_components;


    /* 3.跳过读取的头文件字节Make a one-row-high sample array that will go away when done with image */
    buffer = (*cinfo.mem->alloc_sarray)
            ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_width, 1);

    //  write_bmp_header(&cinfo, output_file);

    output_buffer = (unsigned char *)malloc(row_width * cinfo.output_height);
    memset(output_buffer, 0, row_width * cinfo.output_height);
    tmp = output_buffer;

    /* 4.Process data由左上角从上到下行行扫描 */
    while (cinfo.output_scanline < cinfo.output_height) {
        (void) jpeg_read_scanlines(&cinfo, buffer, 1);
        memcpy(tmp, *buffer, row_width);
        tmp += row_width;
    }

    width = cinfo.output_width ;
    height = cinfo.output_height ;

    int ImageSize = width * height * 3 ;

    imageId.ptr_rgb = output_buffer ;
    imageId.length = ImageSize ;

}

void ReadImage::readimagedata() {

    std::string posfix = GetFilePosfix(imagepath);

    if(posfix==std::string("jpg")) {    //如果是jpg文件，则调用读取jpg API
        readjpgdata() ;
    }
    else if(posfix==std::string("png")) {  //如果是png文件，则调用读取png API
        readpngdata();
    }
    else {
        std::cout << "image format error !" << std::endl ;
    }

}

