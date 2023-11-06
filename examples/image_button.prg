/*
 *
 */

#include "hbgl.ch"

PROCEDURE Main()

   LOCAL app := WindowNew( 830, 450, "Test image button" )
   LOCAL imgBtn := ImageButtonNew( app, "../docs/assets/img/harbour_logo.png", { || openURL("https://github.com/dev-harbour/hbgl" ) } )
   LOCAL startX, startY

   Background( app, 0xF1F1F1 )

   DO WHILE( ! MainLoop( app ) )

      BeginDrawing( app )

         startX := ( GetWindowWidth( app )  - GetImageWidth( imgBtn ) ) / 2
         startY := ( GetWindowHeight( app ) - GetImageHeight( imgBtn ) ) / 2

         DrawImageButton( imgBtn, startX, startY, GetImageWidth( imgBtn ), GetImageHeight( imgBtn ) )

      EndDrawing( app )
      WaitEvents()
   ENDDO

   CloseAll( app )
RETURN
