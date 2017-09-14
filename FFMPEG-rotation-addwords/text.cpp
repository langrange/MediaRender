//
// Created by yfei on 17-8-23.
//
#include "text.h"

#include <fstream>
#include <sstream>
#include <algorithm>

#include "freetype/ftglyph.h"
#include "ReadImage.h"
#include "picture.h"

void text::GetJsonData() {

    std::ifstream in(FrameFormatfile_path, std::ios::binary);
    if (!in.is_open()) {
        std::cout << "Error opening file\n";
    }
    reader.parse(in, root) ;
    in.close();

}

//char2hex
unsigned char Char2Hex(const char C)
{
    unsigned char Data;
    if (C >= '0' && C <= '9')
    {
        Data = C - '0';
    }
    else if (C >= 'A' && C <= 'Z')
    {
        Data = C - 'A' + 10;
    }
    else if (C >= 'a' && C <= 'z')
    {
        Data = C - 'a' + 10;
    }
    return Data;
}

//************************************************************************
//  Function: Utf8ToUnicode
//************************************************************************/
static void Utf8ToUnicode(short * unicode, short *utf8)
{
    int nBytes = 0;

    if (0 == (*utf8 & 0x80))
    {
        /*
         * single-byte char
         */
        nBytes = 1;
        unicode[0] = 0 ;
        unicode[1] = *utf8 ;
    }
    else
    {        /*
         * 3-byte char (chinese char)
         */
        int i;

        if ( (*utf8 & 0xf0) == 0xe0 )
        {
            nBytes  = 3;
            unicode[0] = ((utf8[0] & 0x0f) <<4) + ((utf8[1] & 0x3c) >>2);
            unicode[1] = ((utf8[1] & 0x03) <<6) + (utf8[2] & 0x3f);
        }
        else
        {
            nBytes = 0;
            unicode[0] = '?';
            return;
        }
    }

    return;
}


void text::CharacterImageGet() {

    GetWordContent( ) ;

  //  ReadImage ImageRead ;  //需添加文字的图片读入
  //  ImageRead.imagepath = imagepath ;
  //  ImageRead.readimagedata() ;
  //  image_width = ImageRead.width;
  //  image_height = ImageRead.height ;
/*
    for(int i=0;i<image_height;i++)
        for(int j=0;j<image_width;j++)
        {
            int dex = (image_height-1-i)*image_width + j;
           // r.push_back(ImageRead.r[dex]);
           // g.push_back(ImageRead.g[dex]);
           // b.push_back(ImageRead.b[dex]);
            r.push_back(0);
            g.push_back(0);
            b.push_back(0);
        }
*/
    /// 文字到图片处理
    font_name2font Talbe[8]={{"AdobeHeitiStd-Regular","../fonts/simhei.ttf"},
                             {"黑体","../fonts/simhei.ttf"},
                             {"粗黑","../fonts/simhei.ttf"},
                             {"KaiTi","../fonts/simhei.ttf"}};

    FT_Library    pFTLib = NULL;
    FT_Face       pFTFace = NULL;
    FT_Error      error;

    for(int i=0;i<8;i++){
        if(strcmp(font_name,Talbe[i].text_font_name) == 0)
        {
            textfomat = Talbe[i].font_name_path ;
            break ;
        }
    }

    int length =  strlen(in_text) ;
    error = FT_Init_FreeType( &pFTLib );              // initialize library
    if (error)
    {
        pFTLib  =   0 ;
        printf( " There is some error when Init Library " );
        return  ;
    }

    wchar_t *unicode_text;// 用于存储unicode字符
    short ch[256];
    short unic[2] ;
    unicode_text = (wchar_t*)calloc(1,sizeof(wchar_t)*(strlen(in_text)*2));// 申请内存
    int j = 0;
   for(int i=0;i<length;++i){

         memset(ch,0,sizeof(ch));  // 所有元素置零
         ch[0] = in_text[i];  // 获取in_text中的第i个元素
         if(ch[0] < 0) {
             //// UTF-8编码的汉字，每byte是负数，因此，可以用来判断是否有汉字
             if(i < strlen(in_text)-2) {// 如果没有到字符串末尾
                 ch[1] = in_text[++i];
                 ch[2] = in_text[++i];
             }
         }

         Utf8ToUnicode( unic,ch) ;
         unsigned short unicode ;
         unicode = unic[0]*256 + unic[1] ;
         unicode_text[j] = unicode  ;// 开始转换编码
         //std::cout << unicode_text[0] << std::endl ;
         ++j;
     }

     int num = j; // 记录字符的数量
     //字体偏移量，用做字体显示

     int bitmap_width_sum=0;
     int bitmap_height_sum=0;

    // picture rgb_(width,height);
    int Imagesize = width * height * 3 ;

    picture rgb_(width,height) ;

    picture image_layer(width,height);

    int line_maxValue[3]={0,0,0} ; int index_line=0 ;

    for (int i_out = 0 ; i_out <num; i_out++) {

         //  FT_Set_Transform( pFTFace, &matrix, &pen );
         error = FT_New_Face( pFTLib, textfomat, 0, &pFTFace );// create face object
         if ( ! error) {

             int i_s=0;
             FT_GlyphSlot  slot;
             slot = pFTFace->glyph;
             // use 50pt at 100dpi
             error = FT_Set_Char_Size(pFTFace, 0, font_size *64 ,0
                     ,0);                // set character size
             error = FT_Set_Pixel_Sizes(
                     pFTFace, // face对象句柄
                     0, // 象素宽度
                     font_size ); // 象素高度
             FT_Glyph glyph= NULL ;
             //  load glyph
             FT_Load_Glyph(pFTFace, FT_Get_Char_Index(pFTFace, unicode_text[i_out]),FT_LOAD_DEFAULT);
             error = FT_Get_Glyph(pFTFace->glyph, &glyph);
             if (!error) {

                 //  获取字符位图
                 if (pFTFace->glyph->format != FT_GLYPH_FORMAT_BITMAP)
                 {
                     FT_Render_Glyph(pFTFace->glyph, FT_RENDER_MODE_NORMAL);
                 }
                 //  convert glyph to bitmap with 256 gray
                 FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
                 FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph) glyph;
                 FT_Bitmap &bitmap = bitmap_glyph->bitmap;
                 FT_Glyph_Metrics  Boundary ;
                 Boundary = pFTFace->glyph->metrics ;
                 double y_delta = Boundary.height/64/2 - font_size/2;

                 int in0 = 0;
                 for (int i=0; i<bitmap.rows; i++) {
                     for (int j=0;j<bitmap.width ;j++ ) {
                         if(bitmap.buffer[ in0++]){
                             int index = ((i+bitmap_height_sum -y_delta ))*width
                                         +j+2+bitmap_width_sum ;
                             rgb_.ptr[index*3]=color_value.r;
                             rgb_.ptr[index*3+1]=color_value.g;
                             rgb_.ptr[index*3+2]=color_value.b;
                         }
                     }
                 }

                 //  rgb_.write("shi.ppm");
                 bitmap_width_sum += (slot->advance.x/64) ;
                 // bitmap_height_sum += font_size ;//slot->advance.y/64 ;
                 if( (( bitmap_width_sum + font_size )>width)||(i_out==num-1) ){
                     line_maxValue[index_line] = bitmap_width_sum ;

                     int x_center_delta = width/2-(line_maxValue[index_line]/2+2);
                     int x_right_delta = width-4- line_maxValue[index_line] ;

                     if(3==align) {    // align on center
                         picture temp_image(rgb_._Bias(x_center_delta)) ;
                         image_layer.operator+(temp_image);
                         bitmap_width_sum = 0;
                         bitmap_height_sum += font_size ;
                         index_line++;
                     }
                     else {
                         if (2 == align) {  //align at right
                             picture temp_image(rgb_._Bias(x_right_delta));
                             image_layer.operator+(temp_image);
                             bitmap_width_sum = 0;
                             bitmap_height_sum += font_size;
                             index_line++;
                         } else {   // align at left
                             picture temp_image(rgb_._Bias(0));
                             image_layer.operator+(temp_image);
                             bitmap_width_sum = 0;
                             bitmap_height_sum += font_size;
                             index_line++;
                         }
                     }

                 }


             }
             //  free glyph
             FT_Done_Glyph(glyph);
             glyph = NULL;
         }

     }

    free(rgb_.ptr) ;
    rgb_.ptr = NULL ;

    imageId.ptr_rgb = image_layer.ptr ;
    imageId.length = image_layer.size ;

   /*
     for(int i=0;i<height;i++)    // 有图片背景的文字图片
         for(int j=0;j<width;j++)
         {
             int index = (i+y_bias)*image_width+ j+ x_bias;
             int index_or = i*width+j;
             r[index]=rgb_text.r[index_or];
             g[index]=rgb_text.g[index_or];
             b[index]=rgb_text.b[index_or];

         }
*/
    //free unicode_text
     free(unicode_text);
     unicode_text = NULL ;
    //  free face
    FT_Done_Face(pFTFace);
    pFTFace = NULL;
     //  free FreeType Lib
     FT_Done_FreeType(pFTLib);
     pFTLib  =  NULL;

}

void text::GetWordContent() {

    GetJsonData() ;
    GetAttribute() ;
    // 文字属性读入
    width = attribute_root[editdatapath]["width"].asInt() ;
    height = attribute_root[editdatapath]["height"].asInt() ;
    x_bias = attribute_root[editdatapath]["x"].asDouble() ;
    y_bias = attribute_root[editdatapath]["y"].asDouble() ;
    align = attribute_root[editdatapath]["align"].asInt() ;
    font_name = attribute_root[editdatapath]["font_name"].asCString();
    if(!attribute_root[editdatapath]["font"].asBool())
        font_size = 30 ;
    else{ font_size = attribute_root[editdatapath]["font_size"].asInt() ;}

    char  * color ;
    color = (char *) attribute_root[editdatapath]["color"].asCString();
    color_value.r = Char2Hex(color[1])*16+Char2Hex(color[2]) ;
    color_value.g = Char2Hex(color[3])*16+Char2Hex(color[4]) ;
    color_value.b = Char2Hex(color[5])*16+Char2Hex(color[6]) ;
    in_text = attribute_root[editdatapath]["content"].asCString();

}

void text::GetAttribute() {

    std::ifstream in(AttributesFile_Path , std::ios::binary);
    if (!in.is_open()) {
        std::cout << "Error opening file\n";
    }
    attribute_reader.parse(in, attribute_root) ;
    in.close();

}