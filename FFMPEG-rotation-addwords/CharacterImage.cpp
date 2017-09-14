//
// Created by yfei on 17-8-24.
//

#include "CharacterImage.h"

#include "ppm.h"
#include "png.h"
#include "ft2build.h"
#include "freetype/ftglyph.h"

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


void CharacterImage::CharacterImageGet() {

    FT_Library    pFTLib = NULL;
    FT_Face       pFTFace = NULL;
    FT_Error      error;
    textfomat = "/home/yfei/Clion_Project/Char2Pic/fonts/simsun.ttf" ;
    int length =  strlen(in_text) ;
    error = FT_Init_FreeType( &pFTLib );              /* initialize library */
    if (error)
    {
        pFTLib  =   0 ;
        printf( " There is some error when Init Library " );
        return  ;
    }

    wchar_t *unicode_text;/* 用于存储unicode字符 */
    short ch[256];
    short unic[2] ;
    unicode_text = (wchar_t*)calloc(1,sizeof(wchar_t)*(strlen(in_text)*2));/* 申请内存 */
    int j = 0;
    for(int i=0;i<length;++i){

        memset(ch,0,sizeof(ch));  /* 所有元素置零 */
        ch[0] = in_text[i];  /* 获取in_text中的第i个元素 */
        if(ch[0] < 0) {
            /* GB2312编码的汉字，每byte是负数，因此，可以用来判断是否有汉字 */
            if(i < strlen(in_text)-2) {/* 如果没有到字符串末尾 */
                ch[1] = in_text[++i];
                ch[2] = in_text[++i];
            }
        }

        Utf8ToUnicode( unic,ch) ;
        unsigned short unicode ;
        unicode = unic[0]*256 + unic[1] ;
        unicode_text[j] = unicode  ;/* 开始转换编码 */
        //std::cout << unicode_text[0] << std::endl ;
        ++j;
    }

    int num = j; /* 记录字符的数量 */
    //字体偏移量，用做字体显示

    int bitmap_width_sum=0;
    int bitmap_height_sum=0;
    //int width =  70*num ;
    ppm A(width,height);
    A.r.clear();
    A.g.clear();
    A.b.clear();

    int i_s=0;
    for (int i_out = 0; i_out <num; i_out++) {
        error = FT_New_Face( pFTLib, textfomat, 0, &pFTFace );/* create face object */
        if ( ! error) {

            /* use 50pt at 100dpi */
            error = FT_Set_Char_Size(pFTFace, 0, font_size * 64,0
                    ,0);                /* set character size */
            error = FT_Set_Pixel_Sizes(
                    pFTFace, /* face对象句柄 */
                    0, /* 象素宽度 */
                    font_size ); /* 象素高度 */

            FT_Glyph glyph= NULL ;
            //  load glyph 'C'
            FT_Load_Glyph(pFTFace, FT_Get_Char_Index(pFTFace, unicode_text[i_out]),FT_LOAD_DEFAULT);
            error = FT_Get_Glyph(pFTFace->glyph, &glyph);
            if (!error) {
                // 6. 获取字符位图
                if (pFTFace->glyph->format != FT_GLYPH_FORMAT_BITMAP)
                {
                    FT_Render_Glyph(pFTFace->glyph, FT_RENDER_MODE_NORMAL);
                }
                //  convert glyph to bitmap with 256 gray
                FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
                FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph) glyph;
                FT_Bitmap &bitmap = bitmap_glyph->bitmap;

                for (int i = 0; i < bitmap.rows; i++) {
                    for (int j = 0; j < bitmap.width; ++j) {
                        if(bitmap.buffer[i * bitmap.width + j]){
                            //像素点存在，就置为红色，其他都为默认的白色
                            //if(num*(font_size+5))

                            int index = (i+2+bitmap_height_sum)*width+j+5+bitmap_width_sum ;
                            A.r[index]=255;
                            A.b[index]=255;

                        }
                    }
                }
                bitmap_width_sum += font_size+5 ;
                ++i_s ;
                if((i_s + 1)*(font_size+5)>width){

                    i_s = 0;
                    bitmap_height_sum += font_size+2 ;
                    bitmap_width_sum = 0;

                }
                //  free glyph
                FT_Done_Glyph(glyph);
                glyph = NULL;
            }
            //  free face
            FT_Done_Face(pFTFace);
            pFTFace = NULL;
        }
    }
    //  free FreeType Lib
    FT_Done_FreeType(pFTLib);
    pFTLib  =  NULL;
    char str[20];
    sprintf(str,"%d.ppm",id) ;
    A.write(str);

}
