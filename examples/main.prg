/*
 *
 */

#include "hbgl.ch"

PROCEDURE Main()

   LOCAL app := WindowNew( 830, 450, "Test window" )

   Background( app, 0xF1F1F1 )

   DO WHILE( ! MainLoop( app ) )

      BeginDrawing( app )

         // code ...

      EndDrawing( app )
      WaitEvents( app )

   ENDDO

   CloseAll( app )

RETURN