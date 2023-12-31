/*
 * Copyright 2023 Rafał Jopek ( rafaljopek at hotmail com )
 */

#ifndef STB_IMAGE_IMPLEMENTATION
   #define STB_IMAGE_IMPLEMENTATION
#endif

#include "hbgl.h"

static ErrLog errorLogs[ MAX_ERROR_LOGS ]; // Tablica do przechowywania informacji o błędach
static int errorLogCount = 0;              // Zmienna śledząca ilość zalogowanych błędów

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
// static
static void cursor_position_callback( GLFWwindow *window, double x, double y )
{
   HBGL *pHBGL = glfwGetWindowUserPointer( window );

   pHBGL->cursorX = x;
   pHBGL->cursorY = y;
}

static void key_callback( GLFWwindow *window, int key, int scancode, int action, int mods )
{
   HBGL *pHBGL = glfwGetWindowUserPointer( window );

   pHBGL->keyCode     = key;
   pHBGL->keyScancode = scancode;
   pHBGL->keyAction   = action;
   pHBGL->keyMods     = mods;
}

static void mouse_button_callback( GLFWwindow *window, int button, int action, int mods )
{
   HBGL *pHBGL = glfwGetWindowUserPointer( window );

   pHBGL->mouseButton = button;
   pHBGL->mouseAction = action;
   pHBGL->mouseMods   = mods;
}

static void window_maximize_callback( GLFWwindow *window, int maximized )
{
   HBGL *pHBGL = glfwGetWindowUserPointer( window );

   pHBGL->winMaximized = maximized;
}

static void window_size_callback( GLFWwindow *window, int width, int height )
{
   HBGL *pHBGL = glfwGetWindowUserPointer( window );

   pHBGL->width  = width;
   pHBGL->height = height;
}

static void set_clear_color_fromHex( unsigned long hexColor )
{
   if( ( hexColor <= 0xffffff ) )
   {
      float r, g, b;
      r = ( ( hexColor >> 16 ) & 0xFF ) / 255.0;
      g = ( ( hexColor >> 8 )  & 0xFF ) / 255.0;
      b = (   hexColor         & 0xFF ) / 255.0;
      glClearColor( r, g, b, 1.0 );
   }
   else if( ( hexColor <= 0xffffffff ) )
   {
      float r, g, b, a;
      r = ( ( hexColor >> 24 ) & 0xFF ) / 255.0;
      g = ( ( hexColor >> 16 ) & 0xFF ) / 255.0;
      b = ( ( hexColor >> 8 )  & 0xFF ) / 255.0;
      a = (   hexColor         & 0xFF ) / 255.0;
      glClearColor( r, g, b, a );
   }
   else
   {
      printf( "Invalid hex value passed \n" );
   }
}

static void printDiagnostics( HBGL *pHBGL )
{
   // Wyczyść ekran
   printf( "\033[2J" );

   // Ustaw kursor na początek ekranu
   printf( "\033[H" );

   printf( "Window Dimensions: %dx%d\n", pHBGL->width, pHBGL->height );
   printf( "Cursor Position: (%.2f, %.2f)\n", pHBGL->cursorX, pHBGL->cursorY );
   printf( "Last Key: %d, Scancode: %d, Action: %d, Mods: %d\n", pHBGL->keyCode, pHBGL->keyScancode, pHBGL->keyAction, pHBGL->keyMods );
   printf( "Last Mouse Button: %d, Action: %d, Mods: %d\n", pHBGL->mouseButton, pHBGL->mouseAction, pHBGL->mouseMods );

   if( pHBGL->winMaximized )
   {
      printf( "Window is: maximized\n" );
   }
   else
   {
      printf( "Window is: normal\n" );
   }

   printf( "Background Color: %#lx\n", pHBGL->background );

   printf( "\n" );
   printf( "Total Fonts: %d\n", pHBGL->fontCount );
      if( pHBGL->failedFontCount > 0 )
      {
         printf( "Failed to load Fonts: %d\n", pHBGL->failedFontCount );
      }

   printf( "Total Images: %d\n", pHBGL->imageCount );
      if( pHBGL->failedImageCount > 0 )
      {
         printf( "Failed to load Images: %d\n", pHBGL->failedImageCount );
      }

   printf( "Total Images button: %d\n", pHBGL->imageButtonCount );
      if( pHBGL->failedImageButtonCount > 0 )
      {
         printf( "Failed to load Images: %d\n", pHBGL->failedImageButtonCount );
      }

   printf( "\n" );
   /* TO DO
   if( pHBGL->fontCount > 0 && pHBGL->fonts[ 0 ] )
   {
      printf( "\n" );
      printf("Font: %d\n", pHBGL->fontCount);
      for( int i = 0; i < pHBGL->fontCount; i++ )
      {
         Font *pFont = pHBGL->fonts[ i ];
         printf( "Font ID: %d\n", pFont->fontID );
         printf( "Texture ID: %u\n", pFont->textureID );
         printf( "x: %d\n", pFont->x );
         printf( "y: %d\n", pFont->y );
      }
   }

   if( pHBGL->imageCount > 0 && pHBGL->images[ 0 ] )
   {
      printf( "\n" );
      printf("Image: %d\n", pHBGL->imageCount);
      for( int i = 0; i < pHBGL->imageCount; i++ )
      {
         Image *pImage = pHBGL->images[ i ];
         printf( "Image ID: %d\n", pImage->imageID );
         printf( "Texture ID: %u\n", pImage->textureID );
         printf( "x: %d\n", pImage->x );
         printf( "y: %d\n", pImage->y );
         printf( "Width: %d\n", pImage->width );
         printf( "Height: %d\n", pImage->height );
         printf( "Channels: %d\n", pImage->channels );
      }
   }
   */
   // Wyświetlanie komunikatów o błędach
   if( errorLogCount > 0 )
   {
      printf( "Error Log:\n" );
      for( int i = 0; i < errorLogCount; i++ )
      {
         printf( "Error %d: %s (code %d) at %s:%d\n",
            i,
            errorLogs[ i ].description,
            errorLogs[ i ].errorCode,
            errorLogs[ i ].file,
            errorLogs[ i ].line );
      }

      // Opcjonalnie, można wyczyścić bufor błędów po wyświetleniu
      // Można też zdecydować, że bufor błędów będzie się czyścił automatycznie po przekroczeniu pewnej ilości zapisanych błędów.
      errorLogCount = 0;
   }

   fflush( stdout );
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
// internal
void CheckHBGLError( HBGLErrorCode error_code, const char *file, int line, const char *format, ... )
{
   char buffer[ 256 ]; // Zakładam, że 256 bajtów wystarczy na opis błędu
   va_list args;
   va_start( args, format );
   vsnprintf( buffer, sizeof( buffer ), format, args );
   va_end( args );

   if( errorLogCount < MAX_ERROR_LOGS )
   {
      errorLogs[ errorLogCount ].errorCode = error_code;
      snprintf( errorLogs[ errorLogCount ].description, sizeof( errorLogs[ errorLogCount ].description ), "%s", buffer );
      snprintf( errorLogs[ errorLogCount ].file, sizeof( errorLogs[ errorLogCount ].file ), "%s", file );
      errorLogs[ errorLogCount ].line = line;
      errorLogCount++;
   }
   /*
    * Zostawiam zakomentowane wywołanie printf, aby móc szybko przywrócić
    * wyświetlanie błędów w konsoli, w przypadku wyłączenia funkcji printDiagnostics.
    * Odkomentuj poniższą linię, jeśli potrzebujesz natychmiastowego logowania błędów do konsoli.
    */
   //printf("[HBGL ERROR] %s (code %d) at %s:%d\n", buffer, error_code, file, line);
}

void CheckOpenGLError( const char *stmt, const char *fname, int line, GLenum *errCode )
{
   GLenum err = glGetError();
   if( err != GL_NO_ERROR )
   {
      printf( "OpenGL error %08x, at %s:%i - for %s\n", err, fname, line, stmt );
      *errCode = err;
   }
   else
   {
     *errCode = GL_NO_ERROR;
   }
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
// API functions
/* WindowNew( width, height, title ) --> pHBGL | NIL*/
HB_FUNC( WINDOWNEW )
{
   if( ! glfwInit() )
   {
      exit( EXIT_FAILURE );
   }

   HBGL *pHBGL = malloc( sizeof( HBGL ) );
   if( ! pHBGL )
   {
      fprintf( stderr, "Memory allocation failed for HBGL structure.\n" );
      hb_ret();
   }

   memset( pHBGL, 0, sizeof( HBGL ) );

   pHBGL->width  = hb_parni( 1 );
   pHBGL->height = hb_parni( 2 );
   pHBGL->title  = hb_parc( 3 );

   // Font
   pHBGL->fonts = NULL;
   pHBGL->fontCount = 0;
   pHBGL->failedFontCount = 0;

   // Image
   pHBGL->images = NULL;
   pHBGL->imageCount = 0;
   pHBGL->failedImageCount = 0;

   // Image button
   pHBGL->imagesButton = NULL;
   pHBGL->imageButtonCount = 0;
   pHBGL->failedImageButtonCount = 0;

   pHBGL->window = glfwCreateWindow( pHBGL->width, pHBGL->height, pHBGL->title, NULL, NULL );
   if( ! pHBGL->window )
   {
      free( pHBGL );
      glfwTerminate();
      hb_ret();
   }

   glfwMakeContextCurrent( pHBGL->window );
   glfwSetWindowUserPointer( pHBGL->window, pHBGL );
   glfwSwapInterval( 1 );

   glfwSetCursorPosCallback( pHBGL->window, cursor_position_callback );
   glfwSetKeyCallback( pHBGL->window, key_callback );
   glfwSetMouseButtonCallback( pHBGL->window, mouse_button_callback );
   glfwSetWindowMaximizeCallback( pHBGL->window, window_maximize_callback );
   glfwSetWindowSizeCallback( pHBGL->window, window_size_callback );

   glfwSetWindowSizeLimits( pHBGL->window, pHBGL->width, pHBGL->height, GLFW_DONT_CARE , GLFW_DONT_CARE );

   hb_retptr( pHBGL );
}

/*
 * Using glfwWaitEventsTimeout() instead of glfwWaitEvents() to allow the application
 * to periodically wake up from the event wait state. This is beneficial for performing
 * background tasks such as updating game state, handling network communications, or
 * carrying out periodic checks within the application. The timeout is set to 0.5 seconds,
 * which is a reasonable interval to keep the application responsive without consuming
 * too much CPU. It strikes a balance between efficiency and responsiveness.
 */
/* MainLoop( pHBGL ) --> NIL */
HB_FUNC( MAINLOOP )
{
   HBGL *pHBGL = hb_parptr( 1 );
   bool state;

   pHBGL->closeFlag = glfwWindowShouldClose( pHBGL->window );
   glfwSetWindowShouldClose( pHBGL->window, GLFW_FALSE );

   state = glfwGetWindowAttrib( pHBGL->window, GLFW_ICONIFIED );
   if( state == T )
   {
      while( state == T )
      {
         state = glfwGetWindowAttrib( pHBGL->window, GLFW_ICONIFIED );

         glfwWaitEventsTimeout( 0.5 );
      }
   }
   hb_retl( pHBGL->closeFlag );
}
/* BeginDrawing( pHBGL ) --> NIL */
HB_FUNC( BEGINDRAWING )
{
   HBGL *pHBGL = hb_parptr( 1 );

   glfwGetFramebufferSize( pHBGL->window, &pHBGL->width, &pHBGL->height );

   glViewport( 0, 0, pHBGL->width, pHBGL->height );
   set_clear_color_fromHex( pHBGL->background );
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   glDisable( GL_CULL_FACE );
   glDisable( GL_DEPTH_TEST );
   glDisable( GL_BLEND );

   glMatrixMode( GL_PROJECTION );
   glLoadIdentity();
   glOrtho( 0, pHBGL->width, pHBGL->height, 0, -1, 1 );
   glMatrixMode( GL_MODELVIEW );
   glLoadIdentity();

   glEnable( GL_TEXTURE_2D );
   glEnable( GL_BLEND );
   glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
}

/* EndDawing( pHBGL ) --> NIL */
HB_FUNC( ENDDRAWING )
{
   HBGL *pHBGL = hb_parptr( 1 );

   printDiagnostics( pHBGL );
   REPORT_OPENGL_ERROR( "end_drawing" );
   glfwSwapBuffers( pHBGL->window );
}

/* CloseAll( HBGL *pHBGL ) */
HB_FUNC( CLOSEALL )
{
   HBGL *pHBGL = hb_parptr( 1 );

   if( pHBGL )
   {
      if( pHBGL->fonts != NULL )
      {
         for( int i = 0; i < pHBGL->fontCount; ++i )
         {
            FreeFont( pHBGL->fonts[ i ] );
            pHBGL->fonts[ i ] = NULL; // Ustawienie wskaźnika na NULL po zwolnieniu
            printf( "Font number %d has been closed successfully.\n", i + 1 );
         }
         free( pHBGL->fonts );
         pHBGL->fonts = NULL; // Zerowanie wskaźnika na tablicę po jej zwolnieniu
      }
      //---
      if( pHBGL->images != NULL )
      {
         for( int i = 0; i < pHBGL->imageCount; ++i )
         {
            FreeImage( pHBGL->images[ i ] );
            pHBGL->images[ i ] = NULL; // Ustawienie wskaźnika na NULL po zwolnieniu
            printf( "Image number %d has been closed successfully.\n", i + 1 );
         }
         free( pHBGL->images );
         pHBGL->images = NULL; // Zerowanie wskaźnika na tablicę po jej zwolnieniu
      }
      //---
      if( pHBGL->imagesButton != NULL )
      {
         for( int i = 0; i < pHBGL->imageButtonCount; ++i )
         {
            FreeImageButton( pHBGL->imagesButton[ i ] );
            pHBGL->imagesButton[ i ] = NULL; // Ustawienie wskaźnika na NULL po zwolnieniu
            printf( "Image button number %d has been closed successfully.\n", i + 1 );
         }
         free( pHBGL->imagesButton );
         pHBGL->imagesButton = NULL; // Zerowanie wskaźnika na tablicę po jej zwolnieniu
      }

      glfwDestroyWindow( pHBGL->window );
      free( pHBGL );
      glfwTerminate();
      hb_retnl( T );
   }
   else
   {
      fprintf( stderr, "Error: CloseWindow called with NULL HBGL pointer.\n" );
      hb_retl( F );
   }
}

/* Background( pHBGL, color ) --> NIL */
HB_FUNC( BACKGROUND )
{
   HBGL *pHBGL = hb_parptr( 1 );

   pHBGL->background = hb_parnl( 2 );
}

/* PollEvents() --> NIL */
HB_FUNC( POLLEVENTS )
{
   glfwPollEvents();
}

/* WaitEvents() --> NIL */
HB_FUNC( WAITEVENTS )
{
   glfwWaitEvents();
}

/* openURL( url ) --> NIL */
HB_FUNC( OPENURL )
{
   const char *url = hb_parc( 1 );

   const int commandBufferSize = 256;
   const int commandPrefixMaxSize = 10; // dla "xdg-open ", "start ", "open "

   if( strlen( url ) > ( commandBufferSize - commandPrefixMaxSize - 1 ) )
   {
      fprintf(stderr, "URL is too long\n");
      return;
   }

   char command[ commandBufferSize ]; // Utworzenie bufora na komendę

#if defined( _WIN32 ) || defined( _WIN64 )
   snprintf( command, sizeof( command ), "start %s", url );
#elif defined( __APPLE__ ) || defined( __MACH__ )
   snprintf( command, sizeof( command ), "open %s", url );
#elif defined( __linux__ )
   snprintf( command, sizeof( command ), "xdg-open %s", url );
#else
   fprintf( stderr, "Unsupported platform\n" );
   return;
#endif

   // Wykonaj komendę i sprawdź wynik
   int result = system( command );
   if( result != 0 )
   {
      fprintf( stderr, "Failed to open URL\n" );
   }
}

/* SetWindowWidth( pHBGL, width ) --> NIL */
HB_FUNC( SETWINDOWWIDTH )
{
   HBGL *pHBGL = hb_parptr( 1 );
   pHBGL->width = hb_parni( 2 );
}

/* SetWindowHeight( pHBGL, height ) --> NIL */
HB_FUNC( SETWINDOWHEIGHT )
{
   HBGL *pHBGL = hb_parptr( 1 );
   pHBGL->height = hb_parni( 2 );
}

/* GetWindowWidth( pHBGL ) --> width */
HB_FUNC( GETWINDOWWIDTH )
{
   HBGL *pHBGL = hb_parptr( 1 );
   hb_retni( pHBGL->width );
}

/* GetWindowHeight( pHBGL ) --> height */
HB_FUNC( GETWINDOWHEIGHT )
{
   HBGL *pHBGL = hb_parptr( 1 );
   hb_retni( pHBGL->height );
}
