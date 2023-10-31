/*
 *
 */

#include "hbgl.ch"

PROCEDURE Main()

   LOCAL app := WindowNew( 830, 450, "Test text" )
   LOCAL font := SystemFontNew( app, "Courier_New" )
   LOCAL i, x, y, font_size, color

   Background( app, 0xF1F1F1 )

   DO WHILE( ! MainLoop( app ) )

      BeginDrawing( app )

         FOR i := 0 TO 20

            x := hb_Rand32() % 830
            y := hb_Rand32() % 450

            font_size := hb_Rand32() % 63 + 10

            color := hb_Rand32() % 0xFFFFFF

            DrawFont( font, x, y, "Hello World!", font_size, color )
         NEXT

      EndDrawing( app )
      WaitEvents()
   ENDDO

   CloseAll( app )
RETURN
