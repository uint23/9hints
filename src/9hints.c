/*
 * 9hints:
 *
 * a plan9 inspired dialogue box which gives you
 * all the system info you want without having an
 * ugly bar or box on your screen all the time!
 *
 * > uint 2025
*/
#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <X11/extensions/Xinerama.h>

#include "9hints.h"
#include "config.h"

void create_window(void);
void draw_modules(void);
void get_geometry(void);
void handle_event(XEvent ev);
unsigned long parse_colour(const char* colour);
void quit(void);
void run(void);
void setup(void);

Display* dpy = NULL;
Window root;
Window revert_win;
Client menu;

int screen = -1;
int scr_width = 0;
int scr_height = 0;
int n_modules = 0;
int revert_to;
Bool running = False;

unsigned long border_colour;
unsigned long background_colour;

void create_window(void)
{
	/* disable other programs managing window */
	XSetWindowAttributes attrs;
	attrs.override_redirect = True;

	menu.win= XCreateWindow(
		dpy, root,
		menu.x, menu.y, WIDTH, HEIGHT, BORDER_WIDTH,
		CopyFromParent, InputOutput, CopyFromParent,
		CWOverrideRedirect, &attrs
	);

	XSetWindowBackground(dpy, menu.win, background_colour);
	XSetWindowBorder(dpy, menu.win, border_colour);

	XSelectInput(
		dpy, menu.win,
		ExposureMask | KeyPressMask |
		ButtonPressMask | StructureNotifyMask
	);

	XMapWindow(dpy, menu.win);
	XFlush(dpy);

	/* starting too prevent input to be set too early */
	XEvent ev;
	do {
		XNextEvent(dpy, &ev);
	} while (ev.type != MapNotify);

	/* wm won't do it because override_redirect=True */
	XSetInputFocus(dpy, menu.win, RevertToNone, CurrentTime);
	(void)XGrabKeyboard( /* some wm's wont give keyboard access */
		dpy, menu.win, True,
		GrabModeAsync, GrabModeAsync, CurrentTime
	);

	draw_modules();
}

void draw_modules(void)
{
	GC gc = XCreateGC(dpy, menu.win, 0, NULL);

	for (int i = 0; i < n_modules; i++) {
		if (modules[i].fn) {
			/* call module */
			modules[i].fn(
				dpy, menu.win, gc,
				modules[i].x, modules[i].y
			);
		}
	}
	XFreeGC(dpy, gc);
}

void get_geometry(void)
{
	if (!XineramaIsActive(dpy)) {
		menu.x = (scr_width - WIDTH) / 2;
		menu.y = (scr_height - HEIGHT) / 2;
		return;
	}

	int n_mons = 0;
	XineramaScreenInfo *info = XineramaQueryScreens(dpy, &n_mons);
	/* fallback */
	if (!info || n_mons <= 0) {
		menu.x = (scr_width - WIDTH) / 2;
		menu.y = (scr_height - HEIGHT) / 2;
		if (info) {
			XFree(info);
		}
		return;
	}

	/* find monitor with cursor */
	Window root_ret;
	Window child_ret;
	int rootx_ret;
	int rooty_ret;
	int winx_ret ;
	int winy_ret ;
	unsigned int mask_ret;

	/* get cursor positioning */
	XQueryPointer(
		dpy, root, &root_ret, &child_ret,
		&rootx_ret, &rooty_ret,
		&winx_ret, &winy_ret, &mask_ret
	);

	int mon;
	for (mon = 0; mon < n_mons; mon++) {

		/* check if cursor coordinates within info[mon] dimentions */
		Bool in_mon =
			rootx_ret >= info[mon].x_org &&
			rootx_ret < info[mon].x_org + info[mon].width &&
			rooty_ret >= info[mon].y_org &&
			rooty_ret < info[mon].y_org + info[mon].height;
		if (in_mon) {
			break;
		}
	}

	menu.x = info[mon].x_org + (info[mon].width - WIDTH) / 2;
	menu.y = info[mon].y_org + (info[mon].height - HEIGHT) / 2;

	XFree(info);
}

void handle_event(XEvent ev)
{
	if (ev.type == Expose) {
		draw_modules();
	}
	else if (ev.type == KeyPress) {
		running = False;
	}

	switch (ev.type) {

	case Expose:
		draw_modules();
		break;

	case KeyPress:
		running = False;
		break;

	}
}

unsigned long parse_colour(const char* colour)
{
	XColor col;
	Colormap col_map = DefaultColormap(dpy, DefaultScreen(dpy));

	/* "#..."->XColor */
	if (!XParseColor(dpy, col_map, colour, &col)) {
		fprintf(stderr, "9menu: cannot parse color %s\n", colour);
		return WhitePixel(dpy, DefaultScreen(dpy));
	}

	/* XColor->pixel */
	if (!XAllocColor(dpy, col_map, &col)) {
		fprintf(stderr, "9menu: cannot allocate color %s\n", colour);
		return WhitePixel(dpy, DefaultScreen(dpy));
	}

	return col.pixel;
}

void quit(void)
{
	XSetInputFocus(dpy, revert_win, revert_to, CurrentTime);
	XUngrabKeyboard(dpy, CurrentTime);

	XDestroyWindow(dpy, menu.win);
	XCloseDisplay(dpy);
}

void run(void)
{
	XEvent ev;
	running = True;
	while (running) {
		XNextEvent(dpy, &ev);
		handle_event(ev);
	}
	quit();
}

void setup(void)
{
	dpy = XOpenDisplay(NULL);
	if (!dpy) {
		puts("failed to open display...");
		exit(EXIT_FAILURE);
	}

	screen = XDefaultScreen(dpy);
	scr_width = XDisplayWidth(dpy, screen);
	scr_height = XDisplayHeight(dpy, screen);

	root = RootWindow(dpy, screen);
	/* store current focused window to restore focus after quit */
	XGetInputFocus(dpy, &revert_win, &revert_to);

	border_colour = parse_colour(BORDER_COLOUR);
	background_colour = parse_colour(BACKGROUND_COLOUR);

	n_modules = sizeof(modules) / sizeof(Module);
}

int main(int argc, char** argv)
{
	if (argc > 1) {
		(void)argv;
		printf(VERSION);
	}

	setup();
	get_geometry();
	create_window();
	run();

	return EXIT_SUCCESS;
}
