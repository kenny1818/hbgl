/*
 *
 */

#include "hbgl.ch"

PROCEDURE Main()

   LOCAL app := WindowNew( 830, 450, "Test Image" )
   LOCAL img := ImageNew( app, "../docs/assets/img/harbour_logo.png" )
   LOCAL startX, startY

   Background( app, 0xF1F1F1 )

   DO WHILE( ! MainLoop( app ) )

      BeginDrawing( app )

         startX := ( GetWindowWidth( app )  - GetImageWidth( img ) ) / 2
         startY := ( GetWindowHeight( app ) - GetImageHeight( img ) ) / 2

         DrawImage( img, startX, startY, GetImageWidth( img ), GetImageHeight( img ) )

      EndDrawing( app )
      WaitEvents( app )
   ENDDO

   CloseAll( app )
RETURN
