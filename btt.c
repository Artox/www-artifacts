/**
 * Copyright 2021 Josua Mayer <josua.mayer@jm0.eu>
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <gpiod.h>

int main(int argc, char *argv[]) {
	struct gpiod_chip *mcp = 0;
	struct gpiod_line *gp3 = 0;
	int ret = 0;
	int val = 0;

	switch(argc) {
		case 2:
			break;
		case 0:
			argv[0] = "btt";
		case 1:
		default:
			fprintf(stderr, "Usage: %s [0|1]\n", argv[0]);
			return 1;
			break;
	}

	if(strncmp("0", argv[1], 2) == 0) {
		val = 0;
	} else if(strncmp("1", argv[1], 2) == 0) {
		val = 1;
	} else {
		fprintf(stderr, "failed to parse argument: %s\n", strerror(EINVAL));
		return 1;
	}

	// explicitly open mcp2221 device
	// TODO: handle multiple devices
	//mcp = gpiod_chip_open_by_name("mcp2221_gpio");
	mcp = gpiod_chip_open_by_label("mcp2221_gpio");
	if(mcp == 0) {
		fprintf(stderr, "failed to open \"mcp2221_gpio\": %s\n", strerror(errno));
		return 1;
	}

	// open gp3
	gp3 = gpiod_chip_get_line(mcp, 3);
	if(gp3 == 0) {
		fprintf(stderr, "failed to open gp3: %s\n", strerror(errno));
		ret = 1;
		goto close_chip;
	}

	// reserve gp3 as output
	if(gpiod_line_request_output(gp3, "btt", 0) != 0) {
		fprintf(stderr, "failed to reserve gp3 as output: %s\n", strerror(errno));
		ret = 1;
		goto close_chip;
	}

	// set gp3 value
	if(gpiod_line_set_value(gp3, val) != 0) {
		fprintf(stderr, "failed to set gp3=%i: %s\n", val, strerror(errno));
		ret = 1;
		goto close_chip;
	}
	fprintf(stdout, "gp3=%i\n", val);

close_chip:
	// close chip
	gpiod_chip_close(mcp);
	gp3 = 0;
	mcp = 0;

	return ret;
}
