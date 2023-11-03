/*
 *
 */

#include "hbgl.ch"

PROCEDURE Main()

   LOCAL app := WindowNew( 830, 830, "Vertically image gallery" )
   LOCAL img1 := ImageNew( app, "../docs/assets/button/long_button-1.png" )
   LOCAL img2 := ImageNew( app, "../docs/assets/button/long_button-2.png" )
   LOCAL img3 := ImageNew( app, "../docs/assets/button/long_button-3.png" )
   LOCAL img4 := ImageNew( app, "../docs/assets/button/long_button-4.png" )
   LOCAL img5 := ImageNew( app, "../docs/assets/button/long_button-5.png" )

   LOCAL spaceBetweenImages := 10
   LOCAL totalImagesHeight, startX, startY

   Background( app, 0x202D48 )

   DO WHILE( ! MainLoop( app ) )

      BeginDrawing( app )

         totalImagesHeight := 5 * GetImageHeight( img1 ) + 4 * spaceBetweenImages // dla pięciu obrazów pionowo
         startX := ( GetWindowWidth( app ) - GetImageWidth( img1 ) ) / 2          // wyśrodkuj w poziomie
         startY := ( GetWindowHeight( app ) - totalImagesHeight ) / 2             // wyśrodkuj w pionie

         // Rysowanie każdego obrazu osobno
         DrawImage( img1, startX, startY, GetImageWidth( img1 ), GetImageHeight( img1 ) )
         DrawImage( img2, startX, startY + GetImageHeight( img1 ) + spaceBetweenImages, GetImageWidth( img2 ), GetImageHeight( img2 ) )
         DrawImage( img3, startX, startY + 2 * ( GetImageHeight( img1 ) + spaceBetweenImages ), GetImageWidth( img3 ), GetImageHeight( img3 ) )
         DrawImage( img4, startX, startY + 3 * ( GetImageHeight( img1 ) + spaceBetweenImages ), GetImageWidth( img4 ), GetImageHeight( img4 ) )
         DrawImage( img5, startX, startY + 4 * ( GetImageHeight( img1 ) + spaceBetweenImages ), GetImageWidth( img5 ), GetImageHeight( img5 ) )

       EndDrawing( app )
       WaitEvents()
   ENDDO

   CloseAll( app )
RETURN
