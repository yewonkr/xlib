// gcc bitmapPattern.c -o output -lX11 && ./output

/*
	The program draw a 100 by 200 pixel base window. An image is created from  
	a bitmap pattern of the character E that had been created externally to 
	this program.
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
	Window baseWindow, wWindow, bWindow, mywindow;
	XSetWindowAttributes myat, wat, bat;
		
	XSizeHints wmsize ;
	XWMHints wmhints ;
	XTextProperty windowName, iconName ;
	XEvent baseEvent, myevent;

	GC mygc, imageGC;
	XGCValues myGCValues, myGCvalues;

	Pixmap pattern, mask;
	XImage *local;
	Cursor cursor;

	char *window_name  = "Image" ;
	char *icon_name = "Im" ;

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
	
	valuemask = CWBackPixel | CWBorderPixel | CWEventMask ;

	mywindow  = XCreateWindow( mydisplay, RootWindow( mydisplay, screen_num ),
			300, 50, 400, 300, 3, 
			DefaultDepth( mydisplay, screen_num ), InputOutput,
			DefaultVisual ( mydisplay, screen_num ), 
			valuemask, &myat ) ;

// 3. give the Window  Manager hints

	wmsize.flags = USPosition | USSize ;
	XSetWMNormalHints ( mydisplay, mywindow, &wmsize ) ;
	wmhints.initial_state = NormalState ;
	wmhints.flags = StateHint ;
	XSetWMHints ( mydisplay, mywindow, &wmhints ) ;
	XStringListToTextProperty (&window_name, 1,  &windowName ) ;
	XSetWMName ( mydisplay, mywindow, &windowName ) ;
	XStringListToTextProperty (&icon_name, 1, &iconName ) ;
	XSetWMIconName ( mydisplay, mywindow, &iconName ) ;

// 4. establish window resources

	pattern =  XCreatePixmapFromBitmapData(mydisplay, mywindow, 
				e_bits, e_width, e_height,
				White, Black, DefaultDepth( mydisplay, screen_num ));

	imageGC = XCreateGC( mydisplay, mywindow, 0, NULL); // mymask, &myGCValues); 	
// 5. create all the other windows needed


// 6. select events for each window


// 7. map the windows

	XMapWindow ( mydisplay, mywindow ) ;

// 8. enter the event loop

	done = 0;

	while( done == 0 )
	{
		XNextEvent ( mydisplay, &myevent ) ;

		switch( myevent.type )
		{
		case Expose:
			XSetForeground( mydisplay, imageGC, Black);
			XSetBackground( mydisplay, imageGC, Red);
			XCopyPlane( mydisplay, pattern, mywindow, imageGC, 0, 0,
					e_width, e_height, 10, 10, 1) ;

			XSetForeground( mydisplay, imageGC, Red);
			XSetBackground( mydisplay, imageGC, Black);
			XCopyPlane( mydisplay, pattern, mywindow, imageGC, 0, 0,
					e_width, e_height, 10, 100, 1) ;

			break;

		case ButtonPress:
			if ( myevent.xbutton.button  == Button1 )
			{
				x = myevent.xbutton.x ;
				y = myevent.xbutton.y ;  
				XSetClipOrigin( mydisplay, imageGC, x, y);
				XCopyPlane( mydisplay, pattern, mywindow, imageGC, 0, 0,
					arrow_width, arrow_height, x, y, 1) ;
			}

			if (myevent.xbutton.button == Button3)
				done = 1;

			break;

		} // end of switch
	} // end of while

// 9. clean up before exiting

	XUnmapWindow ( mydisplay, mywindow) ;
	XDestroyWindow ( mydisplay, mywindow) ;
	XCloseDisplay ( mydisplay ) ;

	return 0;
}

