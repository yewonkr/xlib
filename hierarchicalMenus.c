// gcc hierarchicalMenus.c -o output -lX11 && ./output

/*
	This program implements hierarchical menus. The base 400x400 pixel window
	contains two menu bar buttons. The button on the left hand side is 
	connected to a menu of three menu items, amd the top one of those menu
	items is connected to another three item menu.
	Each menu item is a blank window which changes colour when the mouse
	pointer moves over it.
	As the mouse pointer enters a menu item winodw, it is highlighted and if a 
	menu leads from it, that is displayed. When the mouse pointer leaves a 
	menu item, it ceases to be hightligted and any munu of menu items leading
	from it are removed from the display.
	The left hand mouse button is used to select a menu to select a munu item.
*/

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // for fn sleep(30);

#include "color_code.h"

int main (int argc, char *argv[])
{
	Display *mydisplay ;
	
	Window baseW, redW, greenW, mywindow, button ;

	struct {
		Window id;
		int homemenu;
		int menudepth;
		int shown;
		int action;
	} W[13] = {
		{0,1,1,0,2},
		{1,1,1,0,1100},
		{2,2,2,0,1000},
		{3,2,2,0,3},
		{4,2,2,0,4},
		{5,3,3,0,1000},
		{6,3,3,0,1000},
		{7,4,3,0,5},
		{8,4,3,0,1100},
		{9,5,4,0,1000},
		{10,5,4,0,1000},
		{11,5,4,0,1000},
		{12,0,0,0,0}
	};
	
	XSetWindowAttributes myat, buttonat;
	XSizeHints wmsize ;
	XWMHints wmhints ;
	XTextProperty windowName, iconName ;
	XEvent baseEvent, myevent ;
	XColor exact, closest;
	GC baseGC, mygc;
	XGCValues myGCValues;

	char *window_name  = "Hierarchy" ;
	char *icon_name = "Hie" ;

	int screen_num, done, status, i, window;
	int x, y;
	unsigned long valuemask, mymask;

// 1. open connection to the server

	mydisplay   =   XOpenDisplay (0) ;

// 2. create a top level window

	screen_num   =   DefaultScreen ( mydisplay ) ;

	// XAllocNamedColor( mydisplay, XDefaultColormap( mydisplay, screen_num ), 
	// 		"yellow", &exact, &closest);
	
	myat.background_pixel = bgGreen; // closest.pixel; 
	myat.border_pixel = Black; // Pixel(mydisplay, screen_num);
	myat.event_mask = ButtonPressMask | ExposureMask | EnterWindowMask;
	mymask = CWBackPixel | CWBorderPixel | CWEventMask ;

	baseW  = XCreateWindow( mydisplay, RootWindow( mydisplay, screen_num ),
			200, 200, 450, 350, 2, 
			DefaultDepth( mydisplay, screen_num ), InputOutput,
			DefaultVisual ( mydisplay, screen_num ), 
			mymask, &myat ) ;

// 3. give the Window  Manager hints

	wmsize.flags = USPosition | USSize ;
	XSetWMNormalHints ( mydisplay, baseW, &wmsize ) ;
	wmhints.initial_state = NormalState ;
	wmhints.flags = StateHint ;
	XSetWMHints ( mydisplay, baseW, &wmhints ) ;
	XStringListToTextProperty (&window_name, 1,  &windowName ) ;
	XSetWMName ( mydisplay, baseW, &windowName ) ;
	XStringListToTextProperty (&icon_name, 1, &iconName ) ;
	XSetWMIconName ( mydisplay,baseW, &iconName ) ;

// 4. establish window resources

	// XAllocNamedColor( mydisplay, XDefaultColormap( mydisplay, screen_num ), 
	// 		"green", &exact, &closest);
	// green = closest.pixel;
	
	// XAllocNamedColor ( mydisplay, XDefaultColormap( mydisplay, screen_num ),   
	// 		"red", &exact, &closest);
	// red = closest.pixel;

	// valuemask = GCForeground | GCBackground;
	// mygc = XCreateGC( mydisplay, baseW, valuemask, &myGCValues); 	

// 5. create all the other windows needed

	myat.background_pixel = White;
	myat.event_mask = ButtonPressMask | ButtonReleaseMask | ExposureMask 
		| EnterWindowMask | LeaveWindowMask;
	mymask = CWBackPixel | CWBorderPixel | CWEventMask ;

	W[0].id = XCreateWindow( mydisplay, baseW,
			50, 50, 90, 20, 2, 
			DefaultDepth( mydisplay, screen_num ), InputOutput,
			DefaultVisual ( mydisplay, screen_num ), 
			mymask, &myat ) ;

	W[1].id = XCreateWindow( mydisplay, baseW,
			250, 100, 70, 30, 2, 
			DefaultDepth( mydisplay, screen_num ), InputOutput,
			DefaultVisual ( mydisplay, screen_num ), 
			mymask, &myat ) ;

	W[2].id = XCreateWindow( mydisplay, baseW,
			70, 60, 90, 20, 2, 
			DefaultDepth( mydisplay, screen_num ), InputOutput,
			DefaultVisual ( mydisplay, screen_num ), 
			mymask, &myat ) ;

	W[3].id = XCreateWindow( mydisplay, baseW,
			70, 80, 90, 20, 2, 
			DefaultDepth( mydisplay, screen_num ), InputOutput,
			DefaultVisual ( mydisplay, screen_num ), 
			mymask, &myat ) ;

	W[4].id = XCreateWindow( mydisplay, baseW,
			70, 100, 90, 20, 2, 
			DefaultDepth( mydisplay, screen_num ), InputOutput,
			DefaultVisual ( mydisplay, screen_num ), 
			mymask, &myat ) ;

	W[5].id = XCreateWindow( mydisplay, baseW,
			140, 90, 60, 10, 2, 
			DefaultDepth( mydisplay, screen_num ), InputOutput,
			DefaultVisual ( mydisplay, screen_num ), 
			mymask, &myat ) ;

	W[6].id = XCreateWindow( mydisplay, baseW,
			140, 100, 60, 10, 2, 
			DefaultDepth( mydisplay, screen_num ), InputOutput,
			DefaultVisual ( mydisplay, screen_num ), 
			mymask, &myat ) ;

	W[7].id = XCreateWindow( mydisplay, baseW,
			140, 110, 60, 10, 2, 
			DefaultDepth( mydisplay, screen_num ), InputOutput,
			DefaultVisual ( mydisplay, screen_num ), 
			mymask, &myat ) ;

	W[8].id = XCreateWindow( mydisplay, baseW,
			140, 50, 90, 20, 2, 
			DefaultDepth( mydisplay, screen_num ), InputOutput,
			DefaultVisual ( mydisplay, screen_num ), 
			mymask, &myat ) ;

	W[9].id = XCreateWindow( mydisplay, baseW,
			200, 110, 100, 30, 2, 
			DefaultDepth( mydisplay, screen_num ), InputOutput,
			DefaultVisual ( mydisplay, screen_num ), 
			mymask, &myat ) ;

	W[10].id = XCreateWindow( mydisplay, baseW,
			200, 140, 100, 30, 2, 
			DefaultDepth( mydisplay, screen_num ), InputOutput,
			DefaultVisual ( mydisplay, screen_num ), 
			mymask, &myat ) ;

	W[11].id = XCreateWindow( mydisplay, baseW,
			200, 170, 100, 30, 2, 
			DefaultDepth( mydisplay, screen_num ), InputOutput,
			DefaultVisual ( mydisplay, screen_num ), 
			mymask, &myat ) ;

	W[8].id = baseW;
			

// 6. select events for each window


// 7. map the windows

	XMapWindow ( mydisplay, baseW ) ;

	for(i=0; i<12; i++)
	{
		XMapWindow(mydisplay, W[i].id);
		W[i].shown = 1;
	}

	// GC gc = XCreateGC(mydisplay, baseW, 0, 0); // valuemask, &values);
	// char buff[50];  // char *buff; - error

	// sprintf(buff, "the color of the window is 0x%x.", buttonat.background_pixel);
	// XDrawString(mydisplay, baseW, gc, 20, 50, buff, strlen(buff));

// 8. enter the event loop
	done   =   0 ;
	while( done == 0 )
	{
		XNextEvent ( mydisplay, &baseEvent ) ;
		window = 1;

		for(i=0; i<13; i++)
		{
			if (W[i].id == baseEvent.xany.window)
			{
				window = i;
				break;
			}
		}

		switch( baseEvent.type )
		{
		case Expose:
			XMapWindow ( mydisplay, baseW ) ;

			for(i=0; i<12; i++)
			{
				if (W[i].shown == 1)
				{
					XMapWindow(mydisplay, W[i].id);
				}
			}

			// XDrawImageString(mydisplay, button, mygc, 35, 15,  
			// 	"Quit", strlen("quit"));
			break;

		case ButtonPress:
			XUngrabPointer( mydisplay, CurrentTime );
			switch( W[window].action )
			{
			case 1000:
				// printf("XBell(mydisplay, 50)\n");
				break;

			case 1100:
				// printf("done = 1\n");
				break;
			}

			if (baseEvent.xbutton.window == baseW)
				done = 1;


			break;

		case ButtonRelease:
			break;

		case EnterNotify:
			if (i == 12)
				for(i=2; i<12; i++)
					W[i].shown == 0;
			else {
				XSetWindowBackground( mydisplay, W[window].id, Red);
				XClearWindow( mydisplay, W[window].id);
				for ( i = 0; i < 12; i++)
				{
					if ( W[i].menudepth  >  W[window].menudepth )
						W[i].shown = 0;
					if ( W[i].homemenu == W[window].action )
						W[i].shown = 1;
				}
			}

			// for(i=0; i<12; i++)
			// 	if (W[i].shown == 1)
			// 		XMapWindow( mydisplay, W[i].id);
			// 	else 
			// 		XUnmapWindow( mydisplay, W[i].id);

			// XFlush(mydisplay);

			break;	

		case LeaveNotify:
			XSetWindowBackground( mydisplay, W[window].id, bgGreen);
			XClearWindow( mydisplay, W[window].id);	
			XUnmapWindow ( mydisplay, W[window].id ) ;	

			break;	

		} // end of switch
	} // end of while

// 9. clean up before exiting
	XUnmapWindow ( mydisplay, baseW ) ;
	XDestroyWindow ( mydisplay, baseW ) ;
	XCloseDisplay ( mydisplay ) ;

	return 0;
}
