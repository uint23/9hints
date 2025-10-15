#include <math.h>
#include <string.h>
#include <time.h>

#include <X11/Xlib.h>

#include "modules.h"

void module_clock(Display* dpy, Window w, GC gc,
	              int x, int y, int width, int height)
{
	(void) width;
	(void) height;
	char buf[16];
	time_t t = time(NULL);
	strftime(buf, sizeof buf, "%H:%M:%S", localtime(&t));
	XDrawString(dpy, w, gc, x, y, buf, strlen(buf));
}
void module_analouge_clock(Display *dpy, Window w, GC gc,
                           int x, int y, int width, int height)
{
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    double pi = 3.141592653589793;

    /* radius and center based on region size */
    int r = (width < height ? width : height) / 2 - 2;
    int cx = (x + width)  / 2;
    int cy = (y + height) / 2;

    /* draw face */
    XDrawArc(dpy, w, gc, cx - r, cy - r, r * 2, r * 2, 0, 360 * 64);

    /* hand angles (radians, 0° at 12 o'clock) */
    double sec_angle  = (tm->tm_sec  / 60.0) * 2 * pi;
    double min_angle  = (tm->tm_min  / 60.0) * 2 * pi + sec_angle / 60;
    double hour_angle = (tm->tm_hour % 12 / 12.0) * 2 * pi + min_angle / 12;

    /* hour hand */
    int hx = cx + (int)(r * 0.5 * sin(hour_angle));
    int hy = cy - (int)(r * 0.5 * cos(hour_angle));
    XDrawLine(dpy, w, gc, cx, cy, hx, hy);

    /* minute hand */
    int mx = cx + (int)(r * 0.75 * sin(min_angle));
    int my = cy - (int)(r * 0.75 * cos(min_angle));
    XDrawLine(dpy, w, gc, cx, cy, mx, my);

    /* seconds hand */
    int sx = cx + (int)(r * 0.9 * sin(sec_angle));
    int sy = cy - (int)(r * 0.9 * cos(sec_angle));
    XDrawLine(dpy, w, gc, cx, cy, sx, sy);
}
