#pragma once

#include "9hints.h"
#include "modules/clock.h"

/* visuals */
#define WIDTH             700
#define HEIGHT            500
#define BORDER_WIDTH      7
#define BORDER_COLOUR     "#000000"
#define BACKGROUND_COLOUR "#fffff6"

Module modules[] = {
/*   function        x       y       update*/
	{draw_clock,     20,     30,     1     },
};
