#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>

#include "9hints.h"

Display* dpy = NULL;
Window menu_window = 0;

int scr_width = 0;
int scr_height = 0;

void create_window(void);
void setup(void);

void create_window(void)
{
	XCreateSimpleWindow()
}

void setup(void)
{
	if (!XOpenDisplay(NULL)) {
		puts("failed to open display...");
		exit(EXIT_FAILURE);
	}

	int screen = XDefaultScreen(dpy);
	scr_width = XDisplayWidth(dpy, screen);
	scr_height = XDisplayHeight(dpy, screen);
}

int main(int argc, char** argv)
{
	if (argc > 1) {
		(void)argv;
		printf(VERSION);
	}

	return EXIT_SUCCESS;
}
