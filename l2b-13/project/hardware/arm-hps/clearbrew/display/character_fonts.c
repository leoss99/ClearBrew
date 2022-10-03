#include "fonts.h"
#include "screens.h"

#include <intelfpgaup/video.h>

/*
	A Note from the ClearBrew Team:
	
	This code was taken from ShreyansK2000/Translaite, from the following files:
		https://github.com/ShreyansK2000/Translaite/blob/master/Display/OutGraphicsCharFont1.c
		https://github.com/ShreyansK2000/Translaite/blob/master/Display/OutGraphicsCharFont2.c
	
	These files are used with some minor modifications, but we cannot take credit for the majority of this work.
	We are grateful to the Translaite team for their contributions to our project.
*/


/*************************************************************************************************
** This function draws a single ASCII character at the coord and colour specified
** it optionally ERASES the background colour pixels to the background colour
** This means you can use this to erase characters
**
** e.g. writing a space character with Erase set to true will set all pixels in the
** character to the background colour
**
*************************************************************************************************/
void print_character_small_font(int x, int y, short fontcolour, short backgroundcolour, int c, int Erase)
{
// using register variables (as opposed to stack based ones) may make execution faster
// depends on compiler and CPU

	register int row, column, theX = x, theY = y ;
	register int pixels ;
	register short theColour = fontcolour  ;
	register int BitMask, theC = c ;

// if x,y coord off edge of screen don't bother

    if(((short)(x) > (short)(screen_x-1)) || ((short)(y) > (short)(screen_y-1)))
        return ;


// if printable character subtract hex 20
	if(((short)(theC) >= (short)(' ')) && ((short)(theC) <= (short)('~'))) {
		theC = theC - 0x20 ;
		for(row = 0; (char)(row) < (char)(7); row ++)	{

// get the bit pattern for row 0 of the character from the software font
			pixels = Font5x7[theC][row] ;
			BitMask = 16 ;

			for(column = 0; (char)(column) < (char)(5); column ++)	{

// if a pixel in the character display it
				if((pixels & BitMask))
					video_pixel(theX+column, theY+row, theColour) ;

				else {
					if(Erase == 1)

// if pixel is part of background (not part of character)
// erase the background to value of variable BackGroundColour

						video_pixel(theX+column, theY+row, backgroundcolour) ;
				}
				BitMask = BitMask >> 1 ;
			}
		}
	}
}

void print_character_medium_font(int x, int y, short colour, short backgroundcolour, int c, int Erase)
{
	register int 	row,
					column,
					theX = x,
					theY = y ;
	register int 	pixels ;
	register short 	theColour = colour  ;
	register int 	BitMask,
					theCharacter = c,
					theRow, theColumn;


    if(((short)(x) > (short)(screen_x-1)) || ((short)(y) > (short)(screen_y-1)))  // if start off edge of screen don't bother
        return ;

	if(((short)(theCharacter) >= (short)(' ')) && ((short)(theCharacter) <= (short)('~'))) {			// if printable character
		theCharacter -= 0x20 ;																			// subtract hex 20 to get index of first printable character (the space character)
		theRow = FONT2_YPIXELS;
		theColumn = FONT2_XPIXELS;

		for(row = 0; row < theRow ; row ++)	{
			pixels = Font10x14[theCharacter][row] ;		     								// get the pixels for row 0 of the character to be displayed
			BitMask = 512 ;							   											// set of hex 200 i.e. bit 7-0 = 0010 0000 0000
			for(column = 0; column < theColumn;   )  	{
				if((pixels & BitMask))														// if valid pixel, then write it
					video_pixel(theX+column, theY+row, theColour) ;
				else {																		// if not a valid pixel, do we erase or leave it along (no erase)
					if(Erase == 1)
						video_pixel(theX+column, theY+row, backgroundcolour) ;
					// else leave it alone
				}
					column ++ ;
				BitMask = BitMask >> 1 ;
			}
		}
	}
}

void print_character_large_font(int x, int y, short colour, short backgroundcolour, int c, int Erase)
{
	register int 	row,
					column,
					theX = x,
					theY = y ;
	register int 	pixels ;
	register short 	theColour = colour  ;
	register int 	BitMask,
					theCharacter = c,
					theRow, theColumn;


    if(((short)(x) > (short)(screen_x-1)) || ((short)(y) > (short)(screen_y-1)))  // if start off edge of screen don't bother
        return ;

	if(((short)(theCharacter) >= (short)(' ')) && ((short)(theCharacter) <= (short)('~'))) {			// if printable character
		theCharacter -= 0x20 ;																			// subtract hex 20 to get index of first printable character (the space character)
		theRow = FONT3_YPIXELS;
		theColumn = FONT3_XPIXELS;

		for(row = 0; row < theRow ; row ++)	{
			pixels = (Font16x27[theCharacter*54 + row*2] << 8) | Font16x27[theCharacter*54 + row*2 + 1];
			BitMask = 32768;							   											// set of hex 200 i.e. bit 7-0 = 0010 0000 0000
			for(column = 0; column < theColumn;   )  	{
				if((pixels & BitMask))														// if valid pixel, then write it
					video_pixel(theX+column, theY+row, theColour) ;
				else {																		// if not a valid pixel, do we erase or leave it along (no erase)
					if(Erase == 1)
						video_pixel(theX+column, theY+row, backgroundcolour) ;
					// else leave it alone
				}
					column ++ ;
				BitMask = BitMask >> 1 ;
			}
		}
	}
}

void print_character_giant_font(int x, int y, short colour, short backgroundcolour, int c, int Erase)
{
	register int 	row,
					column,
					theX = x,
					theY = y ;
	register long 	pixels ;
	register short 	theColour = colour  ;
	register long 	BitMask,
					theCharacter = c,
					theRow, theColumn;


    if(((short)(x) > (short)(screen_x-1)) || ((short)(y) > (short)(screen_y-1)))  // if start off edge of screen don't bother
        return ;

	if(((short)(theCharacter) >= (short)(' ')) && ((short)(theCharacter) <= (short)('~'))) {			// if printable character
		theCharacter -= 0x20 ;																			// subtract hex 20 to get index of first printable character (the space character)
		theRow = FONT4_YPIXELS;
		theColumn = FONT4_XPIXELS;

		for(row = 0; row < theRow ; row ++)	{
			pixels = (Font22x40[theCharacter*120 + row*3] << 16) | (Font22x40[theCharacter*120 + row*3 + 1] << 8) | Font22x40[theCharacter*120 + row*3 + 2];		     								// get the pixels for row 0 of the character to be displayed
			BitMask = 2097152;
			for(column = 0; column < theColumn;   )  	{
				if((pixels & BitMask))														// if valid pixel, then write it
					video_pixel(theX+column, theY+row, theColour) ;
				else {																		// if not a valid pixel, do we erase or leave it along (no erase)
					if(Erase == 1)
						video_pixel(theX+column, theY+row, backgroundcolour) ;
					// else leave it alone
				}
					column ++ ;
				BitMask = BitMask >> 1 ;
			}
		}
	}
}

void print_character_massive_font(int x, int y, short colour, short backgroundcolour, int c, int Erase)
{
	register int 	row,
					column,
					theX = x,
					theY = y ;
	register int 	pixels ;
	register short 	theColour = colour  ;
	register int 	theCharacter = c,
					theRow, theColumn;
	register unsigned int BitMask;


    if(((short)(x) > (short)(screen_x-1)) || ((short)(y) > (short)(screen_y-1)))  // if start off edge of screen don't bother
        return ;

	if(((short)(theCharacter) >= (short)(' ')) && ((short)(theCharacter) <= (short)('~'))) {			// if printable character
		theCharacter -= 0x20 ;																			// subtract hex 20 to get index of first printable character (the space character)
		theRow = FONT5_YPIXELS;
		theColumn = FONT5_XPIXELS;

		for(row = 0; row < theRow ; row ++)	{
			pixels = (Font38x59[theCharacter*236 + row*4] << 24) | (Font38x59[theCharacter*236 + row*4 + 1] << 16)
			| (Font38x59[theCharacter*236 + row*4 + 2] << 8) | Font38x59[theCharacter*236 + row*4 + 3] ;		     								// get the pixels for row 0 of the character to be displayed
			BitMask = 0x80000000 ;
			for(column = 0; column < theColumn;   )  	{
				if((pixels & BitMask))														// if valid pixel, then write it
					video_pixel(theX+column, theY+row, theColour) ;
				else {																		// if not a valid pixel, do we erase or leave it along (no erase)
					if(Erase == 1)
						video_pixel(theX+column, theY+row, backgroundcolour) ;
					// else leave it alone
				}
					column ++ ;
				BitMask = BitMask >> 1 ;
			}
		}
	}
}