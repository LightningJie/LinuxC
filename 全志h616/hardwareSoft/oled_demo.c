/*
 * Copyright (c) 2015, Vladimir Komendantskiy
 * MIT License
 *
 * SSD1306 demo of block and font drawing.
 */

//
// fixed for OrangePiZero by HypHop
//

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#include "oled.h"
#include "font.h"

int oled_show(struct display_info *disp) {
	int i;
	char buf[100];

	oled_putstrto(disp, 0, 9+1, " Welcome to My Home");
	disp->font = font3;
	oled_putstrto(disp, 0, 23, "    ---wangwenjie---   ");
	disp->font = font3;
	oled_send_buffer(disp);

	return 0;
}


int main(int argc, char **argv) {
	int e;
	char filename[32];
	struct display_info disp;

	memset(&disp, 0, sizeof(disp));
	sprintf(filename, "%s", argv[1]);
	disp.address = OLED_I2C_ADDR;
	disp.font = font2;

	e = oled_open(&disp, filename);
	e = oled_init(&disp);
	oled_clear(&disp);
	oled_show(&disp);
	return 0;
}
