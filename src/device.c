#include "device.h"
#include <stdint.h>
#include <stdio.h>

void console_in(uint8_t input) { printf("%c", input); }

// basically printing
// idk how im going to get input from the user
Device console = {.id = 0x0, .in = console_in, .init = NULL, .out = NULL};

// super simple 255x255 display
// display output isn't meaningful to the cpu
// so no output function here either
Device screen = {
    .id = 0x1,
    .in = NULL,
    .out = NULL,
};
