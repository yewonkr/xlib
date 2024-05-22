// gcc redWindowWithPointer.c -o output -lX11 && ./output

/*
	The program displays a window coloured red. When the right hand mouse
	button is pressed while the pointer is in that window, a pattern patch is
	displayed at the location of the pointer. The pattern is of an arrow
	pointing to the top left which is coloured black, surrounded by a thin 
	white border.
*/

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // for fn sleep(30);

#include "shapes.h"
#include "color_code.h"

int main (int argc, char *argv[])
{
	Display *mydisplay ;
	Window baseWindow, wWindow, bWindow;
	XSetWindowAttributes myat, wat, bat;
		
	XSizeHints wmsize ;
	XWMHints wmhints ;
	XTextProperty windowName, iconName ;
	XEvent baseEvent ;
	XColor exact, closest, front, backing;
	GC mygc;
	XGCValues myGCValues;

	Pixmap pattern, mask;
	Cursor cursor;

	char *window_name  = "transparent" ;
	char *icon_name = "Tr" ;

	int screen_num,  done, count;
	int x, y;
	unsigned long valuemask, mymask ;

// 1. open connection to the server

	mydisplay   =   XOpenDisplay (0) ;

// 2. create a top level window

	screen_num   =   DefaultScreen ( mydisplay);

	myat.background_pixel = Red; // colourBits[0]; 
	myat.border_pixel = Black; // BlackPixel(mydisplay, screen_num);
	
	myat.event_mask = ButtonPressMask | EnterWindowMask | LeaveWindowMask
				| ExposureMask | ButtonReleaseMask | PointerMotionMask ;
	
	mymask = CWBackPixel | CWBorderPixel | CWEventMask ;

	baseWindow  = XCreateWindow( mydisplay, RootWindow( mydisplay, screen_num ),
			300, 300, 450, 350, 3, 
			DefaultDepth( mydisplay, screen_num ), InputOutput,
			DefaultVisual ( mydisplay, screen_num ), 
			mymask, &myat ) ;

// 3. give the Window  Manager hints

	wmsize.flags = USPosition | USSize ;
	XSetWMNormalHints ( mydisplay, baseWindow, &wmsize ) ;
	wmhints.initial_state = NormalState ;
	wmhints.flags = StateHint ;
	XSetWMHints ( mydisplay, baseWindow, &wmhints ) ;
	XStringListToTextProperty (&window_name, 1,  &windowName ) ;
	XSetWMName ( mydisplay, baseWindow, &windowName ) ;
	XStringListToTextProperty (&icon_name, 1, &iconName ) ;
	XSetWMIconName ( mydisplay, baseWindow, &iconName ) ;

// 4. establish window resources

	pattern =  XCreatePixmapFromBitmapData(mydisplay, baseWindow, 
				arrow_bits, arrow_width, arrow_height,
				White, Black, DefaultDepth( mydisplay, screen_num ));

	mask =  XCreatePixmapFromBitmapData(mydisplay, baseWindow, 
				mask_bits, mask_width, mask_height,
				1, 0, 1);

	mymask = GCForeground | GCBackground | GCClipMask ;

	myGCValues.background = White; 
	myGCValues.foreground = Black; 
	myGCValues.clip_mask = mask;

	mygc = XCreateGC( mydisplay, baseWindow, mymask, &myGCValues); 	


// 5. create all the other windows needed


// 6. select events for each window


// 7. map the windows

	XMapWindow ( mydisplay, baseWindow ) ;

// 8. enter the event loop

	done = 0;
	char pos[15];

	while( done == 0 )
	{
		XNextEvent ( mydisplay, &baseEvent ) ;

		switch( baseEvent.type )
		{
		case Expose:
			break;

		case ButtonPress:
			if ( baseEvent.xbutton.button  == Button1 )
			{
				x = baseEvent.xbutton.x ;
				y = baseEvent.xbutton.y ;  
				XSetClipOrigin( mydisplay, mygc, x, y);
				XCopyPlane( mydisplay, pattern, baseWindow, mygc, 0, 0,
					arrow_width, arrow_height, x, y, 1) ;

                sprintf(pos, "(%d %d)", x, y);
                // XSetForeground(mydisplay, mygc, 0xffffff);
                XDrawString(mydisplay, baseWindow, mygc, x, y, pos, strlen(pos));

			}

			if (baseEvent.xbutton.button == Button3)
				done = 1;

			break;

		} // end of switch
	} // end of while

// 9. clean up before exiting

	XUnmapWindow ( mydisplay, baseWindow) ;
	XDestroyWindow ( mydisplay, baseWindow) ;
	XCloseDisplay ( mydisplay ) ;

	return 0;
}

