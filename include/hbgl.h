#ifndef HBGL_H_
#define HBGL_H_

#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GLFW/glfw3.h>

#include "stb_truetype.h"
#include "stb_image.h"

#include "hbapi.h"
#include "hbapierr.h"
#include "hbapiitm.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif

#define MAX_ERROR_LOGS 100 // Liczba błędów do zalogowania

typedef enum
{
   HBGL_SUCCESS = 0,
   HBGL_ERROR_WINDOW_CREATION_FAILED,

   HBGL_ERROR_LOAD_IMAGE_FAILED,

   HBGL_ERROR_FONT_UTF8_DECODE,
   HBGL_ERROR_FONT_FILE_NOT_FOUND,
   HBGL_ERROR_FONT_MEMORY_ALLOCATION,
   HBGL_ERROR_FONT_ARRAY_REALLOCATION,
   HBGL_ERROR_FONT_FILE_OPEN,
   HBGL_ERROR_FONT_FILE_READ,
   HBGL_ERROR_FONT_INITIALIZATION,
   HBGL_ERROR_FONT_DRAW_FONT_FAILED,
   HBGL_ERROR_FONT_INVALID_COLOR_VALUE,

   HBGL_ERROR_OUT_OF_MEMORY,
   // ...
} HBGLErrorCode;

typedef enum bool
{
   F = 0,
   T = ( ! 0 )
} bool;

typedef struct _HBGL HBGL;
typedef struct _ErrLog ErrLog;
typedef struct _Font Font;
typedef struct _Image Image;

struct _ErrLog
{
   HBGLErrorCode errorCode;
   char description[ 256 ];
   char file[ 256 ];
   int line;
};

struct _Font
{
   HBGL *pHBGL;
   unsigned int textureID;
   int fontID;
   int x;
   int y;
   stbtt_bakedchar cdata[ 380 ];
};

struct _Image
{
   HBGL *pHBGL;
   unsigned int textureID;
   int imageID;
   int x;
   int y;
   int width;
   int height;
   int channels;
};

struct _HBGL
{
   GLFWwindow   *window;       // handle of the created window
   int           width;        // desired width, in screen coordinates, of the window
   int           height;       // desired height, in screen coordinates
   const char   *title;        // initial, UTF-8 encoded window title
   bool          closeFlag;    // returns the value of the close flag of the specified window

   double        cursorX;      // new cursor x-coordinate, relative to the left edge of the content area
   double        cursorY;      // new cursor y-coordinate, relative to the top edge of the content area
   int           keyCode;      // the keyboard key that was pressed or released
   int           keyScancode;  // the system-specific scancode of the key
   int           keyAction;    // GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT. Future releases may add more actions
   int           keyMods;      // bit field describing which modifier keys were held down
   int           mouseButton;  // mouse button that was pressed or released
   int           mouseAction;  // one of GLFW_PRESS or GLFW_RELEASE, future releases may add more actions
   int           mouseMods;    // bit field describing which modifier keys were held down
   int           winMaximized; // GLFW_TRUE if the window was maximized, or GLFW_FALSE if it was restored

   unsigned long background;

   Font **fonts;               // Pointer to an array of Font structures
   int fontCount;              // Number of fonts loaded
   int failedFontCount;        // Unloaded font counter

   Image **images;             // Pointer to an array of Image structures
   int imageCount;             // Number of images loaded
   int failedImageCount;       // Unloaded image counter
};

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */

HB_EXTERN_BEGIN

extern HB_EXPORT void FreeFont( Font *pFont );
extern HB_EXPORT void FreeImage( Image *pImage );

extern HB_EXPORT void CheckOpenGLError( const char *stmt, const char *fname, int line, GLenum *errCode );
extern HB_EXPORT void CheckHBGLError( HBGLErrorCode error_code, const char *file, int line, const char *format, ... );

extern HB_EXPORT void Point( int x, int y, unsigned int color );
extern HB_EXPORT void PointSize( int x, int y, int point_size, unsigned int color );
extern HB_EXPORT void Line( int x1, int y1, int x2, int y2, unsigned int color );
extern HB_EXPORT void LineWidth( int x1, int y1, int x2, int y2, int line_width, unsigned int color );
extern HB_EXPORT void Rect( int x1, int y1, int x2, int y2, unsigned int color );
extern HB_EXPORT void RectWidth( int x1, int y1, int x2, int y2, int line_width, unsigned int color );
extern HB_EXPORT void FillRect( int x1, int y1, int x2, int y2, unsigned int color );

HB_EXTERN_END

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
// macros
#define REPORT_OPENGL_ERROR( stmt ) \
   GLenum errCode; \
   CheckOpenGLError( stmt, __FILE__, __LINE__, &errCode ); \
   if( errCode != GL_NO_ERROR ) return;

#define REPORT_HBGL_ERROR( error_code, description, ... ) \
   do { \
      HBGLErrorCode code = ( error_code ); \
      if( code != HBGL_SUCCESS ) { \
         CheckHBGLError( code, __FILE__, __LINE__, description, ##__VA_ARGS__ ); \
      } \
   } while( 0 )

#define HB_ERR_ARGS() ( hb_errRT_BASE_SubstR( EG_ARG, 1004, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS ) )

#define MAX( a, b ) ( ( a ) < ( b ) ? ( b ) : ( a ) )
#define MIN( a, b ) ( ( a ) < ( b ) ? ( a ) : ( b ) )
#define UNUSED( n ) ( ( void )( n ) )

#endif /* End HBGL_H_ */
