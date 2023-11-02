/*
 *
 */

#include "hbgl.ch"

PROCEDURE Main()

   LOCAL app := WindowNew( 830, 450, "Horizontal image gallery" )
   LOCAL img1 := ImageNew( app, "../docs/assets/button/button-1.png" )
   LOCAL img2 := ImageNew( app, "../docs/assets/button/button-2.png" )
   LOCAL img3 := ImageNew( app, "../docs/assets/button/button-3.png" )
   LOCAL img4 := ImageNew( app, "../docs/assets/button/button-4.png" )
   LOCAL img5 := ImageNew( app, "../docs/assets/button/button-5.png" )

   LOCAL spaceBetweenImages := 10
   LOCAL totalImagesWidth, startX, startY

   Background( app, 0x324032 )

   DO WHILE( ! MainLoop( app ) )

      BeginDrawing( app )

         totalImagesWidth := 5 * 128 + 4 * spaceBetweenImages       // dla pięciu obrazów
         startX := ( GetWindowWidth( app ) - totalImagesWidth ) / 2 // wyśrodkuj wszystkie obrazy
         startY := ( GetWindowHeight( app ) - 128 ) / 2

         // Rysowanie każdego obrazu osobno
         DrawImage( img1, startX, startY, 128, 128 )
         DrawImage( img2, startX + 128 + spaceBetweenImages, startY, 128, 128 )
         DrawImage( img3, startX + 2 * ( 128 + spaceBetweenImages ), startY, 128, 128 )
         DrawImage( img4, startX + 3 * ( 128 + spaceBetweenImages ), startY, 128, 128 )
         DrawImage( img5, startX + 4 * ( 128 + spaceBetweenImages ), startY, 128, 128 )

      EndDrawing( app )
      WaitEvents()
   ENDDO

   CloseAll( app )
RETURN
