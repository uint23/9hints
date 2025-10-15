#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>

#include "9hints.h"
#include "config.h"

Display* dpy = NULL;
Window menu_window = 0;

int scr_width = 0;
int scr_height = 0;

unsigned long border_colour;
unsigned long background_colour;

void create_window(void);
unsigned long parse_colour(const char* colour);
void setup(void);

void create_window(void)
{
	int x = scr_width - WIDTH;
	int y = scr_height - HEIGHT;
	XCreateSimpleWindow(dpy, menu_window, x, y, WIDTH, HEIGHT,
			            BORDER_WIDTH, border_colour, background_colour);
	while (1);
}

unsigned long parse_colour(const char* colour)
{
	XColor col;
	Colormap col_map = DefaultColormap(dpy, DefaultScreen(dpy));

	if (!XParseColor(dpy, col_map, colour, &col)) {
		fprintf(stderr, "9menu: cannot parse color %s\n", colour);
		return WhitePixel(dpy, DefaultScreen(dpy));
	}

	if (!XAllocColor(dpy, col_map, &col)) {
		fprintf(stderr, "9menu: cannot allocate color %s\n", colour);
		return WhitePixel(dpy, DefaultScreen(dpy));
	}

	return col.pixel;
}

void setup(void)
{
	dpy = XOpenDisplay(NULL);
	if (!dpy) {
		puts("failed to open display...");
		exit(EXIT_FAILURE);
	}

	int screen = XDefaultScreen(dpy);
	scr_width = XDisplayWidth(dpy, screen);
	scr_height = XDisplayHeight(dpy, screen);

	border_colour = parse_colour(BORDER_COLOUR);
	border_colour = parse_colour(BORDER_COLOUR);
}

int main(int argc, char** argv)
{
	if (argc > 1) {
		(void)argv;
		printf(VERSION);
	}

	setup();
	create_window();

	return EXIT_SUCCESS;
}
