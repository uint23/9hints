#pragma once

#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>

void module_analouge_clock(Display* dpy, Window w, GC gc,
		                   int x, int y, int width, int height);

void module_9hints_banner(Display* dpy, Window w, GC gc,
		                  int x, int y, int width, int height);

void module_clock(Display* dpy, Window w, GC gc,
		          int x, int y, int width, int height);
