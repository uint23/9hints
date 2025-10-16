#include <math.h>
#include <string.h>
#include <time.h>

#include <X11/Xft/Xft.h>
#include <X11/Xlib.h>

#include "modules.h"
#include "config.h"

void module_9hints_banner(Display* dpy, Window w, GC gc,
		                  int x, int y, int width, int height)
{
    (void) gc;
    (void) height;
    const char* message = MESSAGE;

    /* TODO: move Xft logic out of modules */
    XftFont* font = XftFontOpenName(dpy, DefaultScreen(dpy), FONT);
    XftColor font_colour;
    XRenderColor render_colour = {0, 0, 0, 0xffff}; /* black */

    XftDraw* draw = XftDrawCreate(
        dpy, w,
        DefaultVisual(dpy, DefaultScreen(dpy)),
        DefaultColormap(dpy, DefaultScreen(dpy))
    );

    XftColorAllocValue(dpy, DefaultVisual(dpy, DefaultScreen(dpy)),
            DefaultColormap(dpy, DefaultScreen(dpy)),
            &render_colour, &font_colour);

    /* centre text */
    x = (x + width) / 2;
    y = (y + width) / 2;

    XftDrawStringUtf8(draw, &font_colour, font, x, y,
                      (const FcChar8*)message, strlen(message));

    XftColorFree(dpy, DefaultVisual(dpy, DefaultScreen(dpy)),
                 DefaultColormap(dpy, DefaultScreen(dpy)), &font_colour);
    XftFontClose(dpy, font);
    XftDrawDestroy(draw);
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

void module_clock(Display* dpy, Window w, GC gc,
	              int x, int y, int width, int height)
{
    (void) gc;
    (void) width;
    (void) height;
    (void) w;
    time_t t = time(NULL);
    char time_buf[32];
    strftime(time_buf, sizeof(time_buf), "%H:%M:%S", localtime(&t));

    /* TODO: move Xft logic out of modules */
    XftFont* font = XftFontOpenName(dpy, DefaultScreen(dpy), FONT);
    XftColor font_colour;
    XRenderColor render_colour = {0, 0, 0, 0xffff}; /* black */

    XftDraw* draw = XftDrawCreate(
        dpy, w,
        DefaultVisual(dpy, DefaultScreen(dpy)),
        DefaultColormap(dpy, DefaultScreen(dpy))
    );

    XftColorAllocValue(dpy, DefaultVisual(dpy, DefaultScreen(dpy)),
            DefaultColormap(dpy, DefaultScreen(dpy)),
            &render_colour, &font_colour);

    /* centre text */
    /* x = x + width / 2; */
    /* y = y + width / 2; */

    XftDrawStringUtf8(draw, &font_colour, font, x, y,
                      (const FcChar8*)time_buf, strlen(time_buf));

    XftColorFree(dpy, DefaultVisual(dpy, DefaultScreen(dpy)),
                 DefaultColormap(dpy, DefaultScreen(dpy)), &font_colour);
    XftFontClose(dpy, font);
    XftDrawDestroy(draw);
}
