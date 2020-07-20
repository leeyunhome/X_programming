# include <X11/Xlib.h>
# include <X11/Xutil.h>
# include <unistd.h>

int main()
{
    Display *mydisplay;
    XSetWindowAttributes myat ;
    Window mywindow, rover;
    XWindowChanges alter ;
    XSizeHints wmsize;
    XWMHints wmhints;
    XTextProperty windowName, iconName ;
    XEvent myevent;
    char *window_name = "Walking";
    char *icon_name = "Wk" ;
    int screen_num , done ;
    unsigned long valuemask ;
    int x , y ;
    /* 1 . open connec t ion to the s e r v e r */
    mydisplay = XOpenDisplay ( " " ) ;
    /* 2 . c r e a t e a top−l e v e l window */
    screen_num = DefaultScreen ( mydisplay ) ;
    myat.background_pixel = WhitePixel( mydisplay , screen_num ) ;
    myat.border_pixel = BlackPixel( mydisplay , screen_num ) ;
    myat.event_mask = ExposureMask | StructureNotifyMask ;
    valuemask = CWBackPixel | CWBorderPixel | CWEventMask ;
    mywindow = XCreateWindow ( mydisplay , RootWindow ( mydisplay , screen_num ) ,
    200,300,350,250,2 ,
    DefaultDepth( mydisplay , screen_num ) , InputOutput ,
    DefaultVisual( mydisplay , screen_num ) ,
    valuemask , &myat ) ;
    // 3 . g ive the Window Manager h in t s
    wmsize.flags = USPosition | USSize ;
    XSetWMNormalHints ( mydisplay , mywindow, &wmsize ) ;
    wmhints.initial_state = NormalState;
    wmhints.flags = StateHint;
    XSetWMHints ( mydisplay , mywindow, &wmhints ) ;
    XStringListToTextProperty(&window_name , 1 , &windowName ) ;
    XSetWMName( mydisplay , mywindow, &windowName ) ;
    XStringListToTextProperty (&icon_name , 1 , &iconName ) ;
    XSetWMIconName ( mydisplay , mywindow, &iconName ) ;
    // 4 . e s t a b l i s h window r e s ou r c e s
    myat.background_pixel = BlackPixel ( mydisplay , screen_num ) ;

    // 5 . c r e a t e a l l the o th e r windows needed
    rover = XCreateWindow( mydisplay , mywindow,
    100 , 30 , 50 , 70 , 2 ,
    DefaultDepth ( mydisplay , screen_num ) , InputOutput ,
    DefaultVisual ( mydisplay , screen_num ) ,
    valuemask , &myat ) ;
    // 6 . s e l e c t even ts f o r each window
    valuemask = CWX | CWY;
    // 7 . map the windows
    XMapWindow( mydisplay , mywindow ) ;
    // 8 . en t e r the even t loop
    done = 0 ;
    x = 11 ; y = 12 ;
    while ( done == 0 ) {
        alter.x = x ;
        alter.y = y ;
        XConfigureWindow ( mydisplay , rover , valuemask , &alter) ;
        XFlush (mydisplay) ;
        XNextEvent ( mydisplay , &myevent ) ;
        switch(myevent.type) {
        case Expose :
            break ;
        case ConfigureNotify :
            XMapWindow( mydisplay , rover ) ;
            sleep ( 3 ) ;
            x += 5 ; y += 6 ;
        }
    }

    // 9 . c l e an up before e x i t i n g

    XUnmapWindow( mydisplay , mywindow ) ;
    XDestroyWindow ( mydisplay , mywindow ) ;
    XCloseDisplay ( mydisplay ) ;
}