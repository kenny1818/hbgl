/*
 *
 */

#include "hbgl.ch"

PROCEDURE Main()

   LOCAL app := WindowNew( 830, 450, "Test Image 8x8" )
   LOCAL img := ImageNew( app, "../docs/assets/img/8x8.png" )
   LOCAL i, j

   Background( app, 0x323232 )

   DO WHILE( ! MainLoop( app ) )

      BeginDrawing( app )

         FOR i := 0 TO GetWindowWidth( app ) STEP 8

            FOR j := 0 TO GetWindowHeight( app ) STEP 8

               DrawImage( img, i, j, 8, 8 )

            NEXT

         NEXT

      EndDrawing( app )
      WaitEvents( app )
   ENDDO

   CloseAll( app )
RETURN

