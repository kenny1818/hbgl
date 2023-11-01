/*
 * Copyright 2023 Rafał Jopek ( rafaljopek at hotmail com )
 */

#include "hbgl.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
// static
static void set_color_fromHex( unsigned long hexColor )
{
   if( ( hexColor <= 0xffffff ) )
   {
      float r, g, b;
      r = ( ( hexColor >> 16 ) & 0xFF ) / 255.0;
      g = ( ( hexColor >> 8 )  & 0xFF ) / 255.0;
      b = (   hexColor         & 0xFF ) / 255.0;
      glColor3f( r, g, b );
   }
   else if( ( hexColor <= 0xffffffff ) )
   {
      float r, g, b, a;
      r = ( ( hexColor >> 24 ) & 0xFF ) / 255.0;
      g = ( ( hexColor >> 16 ) & 0xFF ) / 255.0;
      b = ( ( hexColor >> 8 )  & 0xFF ) / 255.0;
      a = ( hexColor & 0xFF )           / 255.0;
      glColor4f( r, g, b, a );
   }
   else
   {
      printf( "Invalid hex value passed \n" );
   }
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */

void Point( int x, int y, unsigned int color )
{
   set_color_fromHex( color );
   glPointSize( 1.0 );
   glBegin( GL_POINTS );
   glVertex2f( x, y + 1 );
   glEnd();
}

void PointSize( int x, int y, int point_size, unsigned int color )
{
   if( point_size < 1 )
   {
      fprintf( stderr, "Point size should be greater than 0.\n" );
      point_size = 1;
   }

   set_color_fromHex( color );
   glPointSize( point_size );
   glBegin( GL_POINTS );
   glVertex2f( x, y + 1 );
   glEnd();
}

void Line( int x1, int y1, int x2, int y2, unsigned int color )
{
   set_color_fromHex( color );
   glBegin( GL_LINES );
   glVertex2f( x1, y1 );
   glVertex2f( x2, y2 );
   glEnd();
}

void LineWidth( int x1, int y1, int x2, int y2, int line_width, unsigned int color )
{
   if( line_width < 1 )
   {
      fprintf( stderr, "Line width should be greater than 0.\n" );
      line_width = 1;
   }

   set_color_fromHex( color );
   glLineWidth( line_width );
   glBegin( GL_LINES );
   glVertex2f( x1, y1 );
   glVertex2f( x2, y2 );
   glEnd();
}

void Rect( int x1, int y1, int x2, int y2, unsigned int color )
{
   set_color_fromHex( color );
   glBegin( GL_LINE_LOOP );
   glVertex2f( x1, y1 );
   glVertex2f( x2, y1 );
   glVertex2f( x2, y2 );
   glVertex2f( x1, y2 );
   glEnd();
}

void RectWidth( int x1, int y1, int x2, int y2, int line_width, unsigned int color )
{
   if( line_width < 1 )
   {
      fprintf( stderr, "Line width should be greater than 0.\n" );
      line_width = 1;
   }

   set_color_fromHex( color );
   glLineWidth( line_width );
   glBegin( GL_LINE_LOOP );
   glVertex2f( x1, y1 );
   glVertex2f( x2, y1 );
   glVertex2f( x2, y2 );
   glVertex2f( x1, y2 );
   glEnd();
}


void FillRect( int x1, int y1, int x2, int y2, unsigned int color )
{
   set_color_fromHex( color );
   glBegin( GL_POLYGON );
   glVertex2f( x1, y1 );
   glVertex2f( x2, y1 );
   glVertex2f( x2, y2 );
   glVertex2f( x1, y2 );
   glEnd();
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */

/* Point( x, y, color ) */
HB_FUNC( POINT )
{
   int x = hb_parni( 1 );
   int y = hb_parni( 2 );
   unsigned int color = hb_parni( 3 );

   Point( x, y, color );
}

/* PointSize( x, y, point_size, color ) */
HB_FUNC( POINTSIZE )
{
   int x = hb_parni( 1 );
   int y = hb_parni( 2 );
   int point_size = hb_parni( 3 );
   unsigned int color = hb_parni( 4 );

   PointSize( x, y, point_size, color );
}

/* Line( x1, y1, x2, y2, color ) */
HB_FUNC( LINE )
{
   int x1 = hb_parni( 1 );
   int y1 = hb_parni( 2 );
   int x2 = hb_parni( 3 );
   int y2 = hb_parni( 4 );
   unsigned int color = hb_parni( 5 );

   Line( x1, y1, x2, y2, color );
}

/* LineWidth( x1, y1, x2, y2, line_width, color ) */
HB_FUNC( LINEWIDTH )
{
   int x1 = hb_parni( 1 );
   int y1 = hb_parni( 2 );
   int x2 = hb_parni( 3 );
   int y2 = hb_parni( 4 );
   int line_width = hb_parni( 5 );
   unsigned int color = hb_parni( 6 );

   LineWidth( x1, y1, x2, y2, line_width, color );
}

/* Rect( x1, y1, x2, y2, color ) */
HB_FUNC( RECT )
{
   int x1 = hb_parni( 1 );
   int y1 = hb_parni( 2 );
   int x2 = hb_parni( 3 );
   int y2 = hb_parni( 4 );
   unsigned int color = hb_parni( 5 );

   Rect( x1, y1, x2, y2, color );
}

/* RectWidth( x1, y1, x2, y2, line_width, color ) */
HB_FUNC( RECTWIDTH )
{
   int x1 = hb_parni( 1 );
   int y1 = hb_parni( 2 );
   int x2 = hb_parni( 3 );
   int y2 = hb_parni( 4 );
   int line_width = hb_parni( 5 );
   unsigned int color = hb_parni( 6 );

   RectWidth( x1, y1, x2, y2, line_width, color );
}

/* FillRect( x1, y1, x2, y2, color ) */
HB_FUNC( FILLRECT )
{
   int x1 = hb_parni( 1 );
   int y1 = hb_parni( 2 );
   int x2 = hb_parni( 3 );
   int y2 = hb_parni( 4 );
   unsigned int color = hb_parni( 5 );

   FillRect( x1, y1, x2, y2, color );
}
