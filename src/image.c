/*
 * Copyright 2023 Rafał Jopek ( rafaljopek at hotmail com )
 */

#define STB_IMAGE_IMPLEMENTATION
#include "hbgl.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
/* ImageNew( pHBGL, image_path ) --> Image | NIL */
HB_FUNC( IMAGENEW )
{
   static int imageCounter = 0;  // Statyczna zmienna do zliczania obrazów

   Image *pImage = malloc( sizeof( Image ) );
   if( ! pImage )
   {
      fprintf( stderr, "Failed to allocate memory for new image.\n" );
      hb_ret();
   }

   memset( pImage, 0, sizeof( Image ) );

   pImage->pHBGL = hb_parptr( 1 );  // Ustawiamy wskaźnik do HBGL
   const char *image_path = hb_parc( 2 );

   // Dodanie obrazu do listy obrazów w HBGL
   pImage->pHBGL->imageCount++;
   pImage->pHBGL->images = realloc( pImage->pHBGL->images, pImage->pHBGL->imageCount * sizeof( Image * ) );
   if( ! pImage->pHBGL->images )
   {
      fprintf( stderr, "Failed to reallocate memory for image array.\n" );
      free( pImage );
      hb_ret();
   }
   pImage->pHBGL->images[ pImage->pHBGL->imageCount - 1 ] = pImage;

   pImage->imageID = ++imageCounter;  // przydzielanie unikatowego ID

   unsigned char *data;
   data = stbi_load( image_path, &pImage->width, &pImage->height, &pImage->channels, 0 );
   if( ! data )
   {
      fprintf( stderr, "Failed to load image: %s\n", image_path );

      // Increment the counter of failed image loads
      pImage->pHBGL->failedImageCount++;

      // Cleanup if image loading failed
      pImage->pHBGL->imageCount--;
      if( pImage->pHBGL->imageCount > 0 )
      {
         pImage->pHBGL->images[ pImage->pHBGL->imageCount ] = NULL;
      }
      else
      {
         free( pImage->pHBGL->images );
         pImage->pHBGL->images = NULL;
      }

      // Returning Image with zeroed dimensions to avoid errors with unloaded images.
      memset( pImage, 0, sizeof( Image ) );
      hb_retptr( pImage );
   }

   glGenTextures( 1, &pImage->textureID );
   glBindTexture( GL_TEXTURE_2D, pImage->textureID );

   glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );  // Recommended by NVIDIA

   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

   if( pImage->channels == 3 )
   {
      glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, pImage->width, pImage->height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
   }
   else if( pImage->channels == 4 )
   {
      glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, pImage->width, pImage->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
   }

   stbi_image_free( data );

   hb_retptr( pImage );
}

/* DrawImage( pImage, x, y, width, height ) --> NIL */
HB_FUNC( DRAWIMAGE )
{
   Image *pImage = hb_parptr( 1 );
   int x = hb_parni( 2 );
   int y = hb_parni( 3 );
   int width = hb_parni( 4 );
   int height = hb_parni( 5 );

   if( pImage && pImage->pHBGL && pImage->pHBGL->images )
   {
      // Aktualizacja pozycji obrazu
      pImage->x = x;
      pImage->y = y;

      bool wasEnabled = glIsEnabled( GL_TEXTURE_2D );
      if( ! wasEnabled )
      {
         glEnable( GL_TEXTURE_2D );
      }

      glBindTexture( GL_TEXTURE_2D, pImage->textureID );

      glBegin( GL_QUADS );
         glTexCoord2f( 0.0f, 1.0f ); glVertex2f( ( float )   x, ( float ) ( y + height ) );
         glTexCoord2f( 1.0f, 1.0f ); glVertex2f( ( float ) ( x + width ), ( float ) ( y + height ) );
         glTexCoord2f( 1.0f, 0.0f ); glVertex2f( ( float ) ( x + width ), ( float ) y );
         glTexCoord2f( 0.0f, 0.0f ); glVertex2f( ( float )   x, ( float ) y );
      glEnd();

      glBindTexture( GL_TEXTURE_2D, 0 );

      if( ! wasEnabled )
      {
         glDisable( GL_TEXTURE_2D );
      }
   }
   else
   {
      // Error code
      hb_ret();
   }
}

/* SetImageWidth( pImage, width ) --> NIL */
HB_FUNC( SETIMAGEWIDTH )
{
   Image *pImage = hb_parptr( 1 );
   int width = hb_parni( 2 );

   if( pImage )
   {
      pImage->width = width;
   }
   else
   {
      hb_ret();
   }
}

/* SetImageHeight( pImage, height ) --> NIL */
HB_FUNC( SETIMAGEHEIGHT )
{
   Image *pImage = hb_parptr( 1 );
   int height = hb_parni( 2 );

   if( pImage )
   {
      pImage->height = height;
   }
   else
   {
      hb_ret();
   }
}

/* GetImageWidth( pImage ) --> width */
HB_FUNC( GETIMAGEWIDTH )
{
   Image *pImage = hb_parptr( 1 );

   if( pImage )
   {
      hb_retni( pImage->width );
   }
   else
   {
      hb_ret();
   }
}

/* GetImageHeight( pIMAGE ) --> height */
HB_FUNC( GETIMAGEHEIGHT )
{
   Image *pImage = hb_parptr( 1 );

   if( pImage )
   {
      hb_retni( pImage->height );
   }
   else
   {
      hb_ret();
   }
}

void FreeImage( Image *pImage )
{
   if( pImage != NULL )
   {
      glDeleteTextures( 1, &pImage->textureID );
      free( pImage );
   }
}
