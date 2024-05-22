// gcc windowWithChildWindow.c -o output -lX11 && ./output

/*
	First a basic window with a white background is created. Then another
	window, a child of the first is created with a black background. This
	second window is repeatedly mapped onto its parent window and then
	removed	after 3 seconds.
*/


#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // for fn sleep(30);

int main (int argc, char *argv[])
{
	Display *mydisplay ;
	XSetWindowAttributes myat ;
	Window mywindow, rover ;
	XWindowChanges alter ;
	XSizeHints wmsize ;
	XWMHints wmhints ;
	XTextProperty windowName, iconName ;
	XEvent myevent ;

	char *window_name  = "Walking" ;
	char *icon_name = "Wk" ;

	int screen_num,  done ;
	unsigned long valuemask ;
	int x, y;

// 1. open connection to the server

	mydisplay   =   XOpenDisplay (0) ;

// 2. create a top level window

	screen_num   =   DefaultScreen ( mydisplay ) ;

	myat.background_pixel = 0x008822; // WhitePixel ( mydisplay, screen_num ) ;
	myat.border_pixel = BlackPixel ( mydisplay, screen_num ) ;
	myat.event_mask = ButtonPressMask | ExposureMask | StructureNotifyMask;
	valuemask = CWBackPixel | CWBorderPixel | CWEventMask ;

	mywindow  = XCreateWindow( mydisplay, RootWindow( mydisplay, screen_num ),
			200, 200, 450, 350, 2, 
			DefaultDepth( mydisplay, screen_num ), InputOutput,
			DefaultVisual ( mydisplay, screen_num ), 
			valuemask, &myat ) ;

// 3. give the Window  Manager hints

	wmsize.flags = USPosition | USSize ;
	XSetWMNormalHints ( mydisplay ,   mywindow ,   &wmsize ) ;
	wmhints.initial_state = NormalState ;
	wmhints.flags = StateHint ;
	XSetWMHints ( mydisplay, mywindow, &wmhints ) ;
	XStringListToTextProperty (&window_name, 1,  &windowName ) ;
	XSetWMName ( mydisplay ,   mywindow ,   &windowName ) ;
	XStringListToTextProperty (&icon_name, 1, &iconName ) ;
	XSetWMIconName ( mydisplay ,   mywindow ,   &iconName ) ;

// 4. establish window resources

	myat.background_pixel =  BlackPixel( mydisplay , screen_num ); 

// 5. create all the other windows needed

	rover = XCreateWindow( mydisplay, mywindow,
			20, 50, 50, 70, 2, 
			DefaultDepth( mydisplay, screen_num ), InputOutput,
			DefaultVisual ( mydisplay, screen_num ), 
			valuemask, &myat ) ;

// 6. select events for each window

	valuemask = CWX | CWY ;

// 7. map the windows

	XMapWindow ( mydisplay, mywindow ) ;

	// XGCValues values = 0;
	// unsigned long valuemask = 0;
	GC gc = XCreateGC(mydisplay, mywindow, 0, 0); // valuemask, &values);
	char buff[50];  // char *buff; - error
	// char *text = "click the mouse to close the window";

	sprintf(buff, "the color of the window is 0x%x.", myat.background_pixel);

	XDrawString(mydisplay, mywindow, gc, 20, 50, buff, strlen(buff));

	printf("click the mouse to close the window\n");


// 8. enter the event loop

	done   =   0 ;
	x = 10; y = 15;
	char pos[50];

	while( done == 0 )
	{
		alter.x = x;
		alter.y = y;

		sprintf(pos, "alter.x = %d, alter.y = %d", alter.x, alter.y );
		// cannot erase the previous line of text - solved by this line		
		XClearWindow( mydisplay, mywindow);
		XDrawString(mydisplay, mywindow, gc, 220, alter.y, pos, strlen(pos));

		XConfigureWindow ( mydisplay, rover, valuemask, &alter); 
		XFlush ( mydisplay ); 

		XNextEvent ( mydisplay, &myevent ) ;
		switch( myevent.type )
		{
		case Expose:
			break;

		case ConfigureNotify:
		 	XMapWindow ( mydisplay, rover ) ;
		 	sleep(1);
		 	x += 5; y += 10;
		 	
		 	if (x > 100)
		 	{
		 		done = 1;
		 		sprintf(pos, "should exit while loop\n");
		 		XDrawString(mydisplay, mywindow, gc, 220, alter.y+10, pos, strlen(pos));
		 	}
		 		
	 		break;
	

		case ButtonPress:
			XDrawString(mydisplay, mywindow, gc, 20, 50, buff, strlen(buff));
	 		// XMapWindow ( mydisplay, mywindow ) ;
			break;

		} // end of switch
	} // end of while

// 9. clean up before exiting
	XUnmapWindow ( mydisplay ,   mywindow ) ;
	XDestroyWindow ( mydisplay ,   mywindow ) ;
	XCloseDisplay ( mydisplay ) ;

	return 0;
}
