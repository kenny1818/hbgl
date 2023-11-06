/*
 * Copyright 2023 Rafał Jopek ( rafaljopek at hotmail com )
 */

#include "hbgl.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
// static
static int is_point_inside( ImageButton *pImageButton, int x, int y )
{
   return x >= pImageButton->x && x <= ( pImageButton->x + ( int ) pImageButton->width ) &&
          y >= pImageButton->y && y <= ( pImageButton->y + ( int ) pImageButton->height );
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
/* ImageButtonNew( pHBGL, image_button_path, onClick ) --> ImageButton | NIL */
HB_FUNC( IMAGEBUTTONNEW )
{

   HBGL *pHBGL = hb_parptr( 1 );
   const char *image_button_path = hb_parc( 2 );

   static int imageButtonCounter = 0;  // Statyczna zmienna do zliczania obrazów

   ImageButton *pImageButton = malloc( sizeof( ImageButton ) );
   if( ! pImageButton )
   {
      fprintf( stderr, "Failed to allocate memory for new image.\n" );
      hb_ret();
   }

   memset( pImageButton, 0, sizeof( ImageButton ) );

   pImageButton->pHBGL = pHBGL;  // Ustawiamy wskaźnik do HBGL

   pImageButton->onClick = hb_itemNew( NULL );
   hb_itemCopy( pImageButton->onClick, hb_param( 3, HB_IT_BLOCK ) );

   // Dodanie obrazu do listy obrazów w HBGL
   pImageButton->pHBGL->imageButtonCount++;
   pImageButton->pHBGL->imagesButton = realloc( pImageButton->pHBGL->imagesButton, pImageButton->pHBGL->imageButtonCount * sizeof( ImageButton * ) );
   if( ! pImageButton->pHBGL->imagesButton )
   {
      fprintf( stderr, "Failed to reallocate memory for image array.\n" );
      free( pImageButton );
      hb_ret();
   }
   pImageButton->pHBGL->imagesButton[ pImageButton->pHBGL->imageButtonCount - 1 ] = pImageButton;

   pImageButton->imageButtonID = ++imageButtonCounter;  // przydzielanie unikatowego ID

   unsigned char *data;
   data = stbi_load( image_button_path, &pImageButton->width, &pImageButton->height, &pImageButton->channels, 0 );
   if( ! data )
   {
      fprintf( stderr, "Failed to load image: %s\n", image_button_path );

      // Increment the counter of failed image loads
      pImageButton->pHBGL->failedImageButtonCount++;

      // Cleanup if image loading failed
      pImageButton->pHBGL->imageButtonCount--;
      if( pImageButton->pHBGL->imageButtonCount > 0 )
      {
         // Set the last valid element of the array to NULL
         pImageButton->pHBGL->imagesButton[ pImageButton->pHBGL->imageButtonCount ] = NULL;
      }
      else
      {
         free( pImageButton->pHBGL->imagesButton );
         pImageButton->pHBGL->imagesButton = NULL;
      }

      // Returning ImageButton with zeroed dimensions to avoid errors with unloaded imagesButton.
      memset( pImageButton, 0, sizeof( ImageButton ) );
      hb_retptr( pImageButton );
   }

   glGenTextures( 1, &pImageButton->textureID );
   glBindTexture( GL_TEXTURE_2D, pImageButton->textureID );

   glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );  // Recommended by NVIDIA

   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

   if( pImageButton->channels == 3 )
   {
      glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, pImageButton->width, pImageButton->height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
   }
   else if( pImageButton->channels == 4 )
   {
      glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, pImageButton->width, pImageButton->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
   }

   stbi_image_free( data );

   hb_retptr( pImageButton );
}

/* DrawImageButton( pImageButton, x, y, width, height ) --> NIL */
HB_FUNC( DRAWIMAGEBUTTON )
{
   ImageButton *pImageButton = hb_parptr( 1 );

   if( pImageButton && pImageButton->pHBGL && pImageButton->pHBGL->imagesButton )
   {
      int x = hb_parni( 2 );
      int y = hb_parni( 3 );
      int width = hb_parni( 4 );
      int height = hb_parni( 5 );

      // Zapisz aktualny kolor i inne atrybuty, które chcesz zachować
      glPushAttrib( GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TEXTURE_BIT );

      // Aktualizacja pozycji obrazu
      pImageButton->x = x;
      pImageButton->y = y;

      if( is_point_inside( pImageButton, pImageButton->pHBGL->cursorX, pImageButton->pHBGL->cursorY ) )
      {
         pImageButton->mouseOver = T;
      }
      else
      {
         pImageButton->mouseOver = F;
      }

      if( pImageButton->pHBGL->mouseButton == GLFW_MOUSE_BUTTON_LEFT && pImageButton->pHBGL->mouseAction == GLFW_PRESS )
      {
         if( pImageButton->mouseOver )
         {
            pImageButton->clicked = T;
            if( pImageButton->onClick )
            {
               hb_vmEvalBlock( pImageButton->onClick );
            }
         }
      }
      else if( pImageButton->pHBGL->mouseButton == GLFW_MOUSE_BUTTON_LEFT && pImageButton->pHBGL->mouseAction == GLFW_RELEASE )
      {
         pImageButton->clicked = F; // Reset the 'clicked' flag after releasing the mouse image button
      }

      if( pImageButton->clicked )
      {
         glColor3f( 1.0f, 1.0f, 1.0f );  // Zwiększ jasność o 10%
      }
      else if( pImageButton->mouseOver )
      {
         glColor3f( 0.9f, 0.9f, 0.9f );  // Zmniejsz jasność o 10%
      }

      bool wasEnabled = glIsEnabled( GL_TEXTURE_2D );
      if( ! wasEnabled )
      {
         glEnable( GL_TEXTURE_2D );
      }

      glBindTexture( GL_TEXTURE_2D, pImageButton->textureID );

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

      // Przywróć poprzedni stan atrybutów
      glPopAttrib();   }
   else
   {
      // Error code
      return;
   }
}

void FreeImageButton( ImageButton *pImageButton )
{
   if( pImageButton )
   {
      // Jeśli pImageButton->onClick jest nie-NULL, zwolnij blok kodu Harbour.
      if( pImageButton->onClick )
      {
         hb_itemRelease( pImageButton->onClick );
      }

      // Sprawdzamy, czy ID tekstury jest ważne, jeśli tak, usuwamy teksturę.
      if( pImageButton->textureID )
      {
         glDeleteTextures(1, &pImageButton->textureID);
      }

      // Zwalniamy pamięć przydzieloną dla struktury ImageButton.
      free( pImageButton );
    }
}
