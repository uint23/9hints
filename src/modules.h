#pragma once

#include <X11/Xlib.h>

void module_clock(Display* dpy, Window w, GC gc,
		          int x, int y, int width, int height);

void module_analouge_clock(Display* dpy, Window w, GC gc,
		                   int x, int y, int width, int height);
