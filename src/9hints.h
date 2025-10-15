#pragma once

#include <X11/Xlib.h>

#define VERSION "9hints ver. 0.1"
#define AUTHOR "(C) Abhinav Prasai 2025"
#define LICINFO "See LICENSE for more info"

typedef struct {
	int    x;
	int    y;
	int    width;
	int    height;
	Window win;
} Client;
