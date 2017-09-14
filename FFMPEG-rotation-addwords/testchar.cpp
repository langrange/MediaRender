#include <string.h>
#include <math.h>
 
#include <ft2build.h>
#include FT_FREETYPE_H
 
 
#define WIDTH   100
#define HEIGHT  30
 
 
/* origin is the upper left corner */
unsigned char image[HEIGHT][WIDTH];
 
 
/* Replace this function with something useful. */
 
void
draw_bitmap( FT_Bitmap*  bitmap,
             FT_Int      x,
             FT_Int      y)
{
  FT_Int  i, j, p, q;
  FT_Int  x_max = x + bitmap->width;
  FT_Int  y_max = y + bitmap->rows;
 
 
  for ( i = x, p = 0; i < x_max; i++, p++ )
  {
    for ( j = y, q = 0; j < y_max; j++, q++ )
    {
      if ( i < 0      || j < 0       ||
           i >= WIDTH || j >= HEIGHT )
        continue;
 
      image[j][i] |= bitmap->buffer[q * bitmap->width + p];
    }
  }
}
 
 
void
show_image( void )
{
  int  i, j;
 
 
  for ( i = 0; i < HEIGHT; i++ )
  {
    for ( j = 0; j < WIDTH; j++ )
      putchar( image[i][j] == 0 ? ' '
                                : image[i][j] < 128 ? '+'
                                                    : '*' );
    putchar( '\n' );
  }
}
 
 
int main( )
{
  FT_Library    library;
  FT_Face       face;
 
  FT_GlyphSlot  slot;
  FT_Matrix     matrix;                 /* transformation matrix */
  FT_Vector     pen;                    /* untransformed origin  */
  FT_Error      error;
 
  char*         filename;
  //char*         text;
 
  double        angle;
  int           target_height;
  int           n, num_chars;

    filename = "/home/yfei/下载/字库/fangsong/超世纪粗仿宋体繁.TTF";
    wchar_t *chinese_str = L"Y/QH";
    unsigned int *chinese_ptr = (unsigned int *)chinese_str;

  const char *text = "good morning";
 // unsigned int *chinese_ptr = (unsigned int *)chinese_str;
 
  num_chars     = strlen( text );
  angle         = ( 0.0 / 360 ) * 3.14159 * 2;      /* use 25 degrees     */
  target_height = HEIGHT;
 
  error = FT_Init_FreeType( &library );              /* initialize library */
  /* error handling omitted */
 
  error = FT_New_Face( library, filename, 0, &face );/* create face object */
  /* error handling omitted */
 
#if 0
  /* use 50pt at 100dpi */
  error = FT_Set_Char_Size( face, 50 * 64, 0,
                            100, 0 );                /* set character size */
#else
  error = FT_Set_Pixel_Sizes(face,0,12);
#endif
 
  /* error handling omitted */
 
  slot = face->glyph;
 
  /* set up matrix */
  matrix.xx = (FT_Fixed)( cos( angle ) * 0x10000L );
  matrix.xy = (FT_Fixed)(-sin( angle ) * 0x10000L );
  matrix.yx = (FT_Fixed)( sin( angle ) * 0x10000L );
  matrix.yy = (FT_Fixed)( cos( angle ) * 0x10000L );
 
  /* the pen position in 26.6 cartesian space coordinates; */
  /* start at (300,200) relative to the upper left corner  */
  pen.x = 0 * 64;
  pen.y = ( target_height - 25 ) * 64;
 
  for ( n = 0; n < 12; n++ )
  {
    /* set transformation */
    FT_Set_Transform( face, &matrix, &pen );
 
    /* load glyph image into the slot (erase previous one) */
    error = FT_Load_Char( face, chinese_ptr[n], FT_LOAD_RENDER );
    if ( error )
      continue;                 /* ignore errors */
 
    /* now, draw to our target surface (convert position) */
    draw_bitmap( &slot->bitmap,
                 slot->bitmap_left,
                 slot->bitmap_top );
 
    /* increment pen position */
    pen.x += slot->advance.x + 12*64;
    pen.y += slot->advance.y + 2*64;
  }
 
  show_image();
 
  FT_Done_Face    ( face );
  FT_Done_FreeType( library );
 
  return 0;
}