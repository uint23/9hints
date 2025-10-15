#include <string.h>
#include <time.h>

#include <X11/Xlib.h>

#include "clock.h"

void draw_clock(Display* dpy, Window w, GC gc, int x, int y)
{
	char buf[16];
	time_t t = time(NULL);
	strftime(buf, sizeof buf, "%H:%M:%S", localtime(&t));
	XDrawString(dpy, w, gc, x, y, buf, strlen(buf));
}
