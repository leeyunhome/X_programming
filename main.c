#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <unistd.h>

enum {
        RECT_X = 20,
        RECT_Y = 20,
        RECT_WIDTH = 10,
        RECT_HEIGHT = 10,

        WIN_X = 10,
        WIN_Y = 10,
        WIN_WIDTH = 200,
        WIN_HEIGHT = 200,
        WIN_BORDER = 1
};

int main() {
        Display *display;
        Window window;
        Window window_sub;
        XEvent event;
        int screen;
        XWindowChanges changes;

        /* open connection with the server */
        display = XOpenDisplay(NULL);
        if (display == NULL) {
                fprintf(stderr, "Cannot open display\n");
                exit(1);
        }

        screen = DefaultScreen(display);

        /* create window */
        window = XCreateSimpleWindow(display, RootWindow(display, screen), WIN_X, WIN_Y, WIN_WIDTH, WIN_HEIGHT,
                WIN_BORDER, BlackPixel(display, screen), WhitePixel(display, screen));

        window_sub = XCreateSimpleWindow(display, RootWindow(display, screen), WIN_X + 10, WIN_Y + 10, WIN_WIDTH + 50, WIN_HEIGHT + 50,
        WIN_BORDER, BlackPixel(display, screen), WhitePixel(display, screen));



        /* process window close event through event handler so XNextEvent does not fail */
        Atom del_window = XInternAtom(display, "WM_DELETE_WINDOW", 0);
        XSetWMProtocols(display, window, &del_window, 1);
        XSetWMProtocols(display, window_sub, &del_window, 1);


        /* select kind of events we are interested in */
        XSelectInput(display, window, ExposureMask | KeyPressMask);
        XSelectInput(display, window_sub, ExposureMask | KeyPressMask);

        // XConfigureWindow(display, window, , &changes);

        /* display the window */
        // XMapWindow(display, window_sub);

        // XRaiseWindow(display, window_sub);
        // XLowerWindow(display, window_sub);
        for (int i = 0; i < 3; ++i)
        {
                XMapWindow(display, window);

                XMoveWindow(display, (int32_t) window, 300 + i, 300 + i);
                sleep(2);
                XMapWindow(display, window);

        }

        /* event loop */
        while (1) {
                XNextEvent(display, &event);

                switch (event.type) {
                        case KeyPress:
                                /* FALLTHROUGH */
                        case ClientMessage:
                                goto breakout;
                        case Expose:
                                /* draw the window */
                                XFillRectangle(display, window, DefaultGC(display, screen), RECT_X, RECT_Y, RECT_WIDTH, RECT_HEIGHT);

                        /* NO DEFAULT */
                }
        }
breakout:

        /* destroy window */
        XDestroyWindow(display, window);
        XDestroyWindow(display, window_sub);


        /* close connection to server */
        XCloseDisplay(display);

        return 0;
}