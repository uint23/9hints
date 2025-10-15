#pragma once

#include "9hints.h"
#include "modules.h"

/* visuals */
#define WIDTH             700
#define HEIGHT            500
#define BORDER_WIDTH      2
#define BORDER_COLOUR     "#000000"
#define BACKGROUND_COLOUR "#fffff6"

Module modules[] = {
/*   function                 x        y,       width,    height */
	{module_analouge_clock,   20,      20,      100,      100    },
	{module_clock,            40,   100+40,  100,      100    },
};
