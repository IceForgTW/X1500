/*
 *  Auto calculate DDR Controller and Synopsys Phy Timing.
 *  Support chip: Jz4780, Jz4775
 */
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <bsp.h>

unsigned long mem_clk = CFG_MPLL_SPEED / CFG_DIV;
unsigned long ps = 0;
int fd;

#include "dwc_ddr_publ.h"
#define CONFIG_DDR_HEAD_FILE "./ddr_timing.h"
#define CONFIG_HEAD "/* Automatically generated - do not edit */ \n\n#ifndef __JZ_GEN_DDR_TIMING_H__\n#define __JZ_GEN_DDR_TIMING_H__\n\n"
#define CONFIG_TAIL "\n#endif //__JZ_GEN_DDR_TIMING_H__\n\n"

static int write_file(int fd, char* data, int len)
{
	if (write(fd, data, len) != strlen(data)) {
		fprintf (stderr, "%s: Write error on %s: %s\n",
			 "gen_ddr_timing", CONFIG_DDR_HEAD_FILE, strerror(errno));
		return (-1);
	}

    return (0);
}

int main(int argc, char* argv[])
{
	int i = 0;
	char *buf;

	ps = 1000000000 / (mem_clk / 1000);
	if (ps == 0)
		return -1;

	/* printf("memory clock = %lu\n", mem_clk); */
	if ((fd = open(CONFIG_DDR_HEAD_FILE, O_RDWR | O_CREAT | O_TRUNC, 0666)) < 0) {
		fprintf (stderr, "%s: Can't open %s: %s\n",
			 "gen_ddr_timing",  CONFIG_DDR_HEAD_FILE, strerror(errno));
		return 0;
	}

	if (write_file(fd, CONFIG_HEAD, strlen(CONFIG_HEAD)) < 0) {
		fprintf (stderr, "%s: Can't write HEAD: %s\n",
			 "gen_ddr_timing", strerror(errno));
		return 0;
	}

	while (1) {
		if (asprintf(&buf, "#define %s\t%lu\n", timing[i].name, gen_ddr_value(timing[i])) < 0) {
			fprintf (stderr, "%s: asprintf alloc fail: %s\n",
				 "gen_ddr_timing", strerror(errno));
			goto err;
		}
		if (write_file(fd, buf, strlen(buf)) < 0) {
			fprintf (stderr, "%s: Can't write context: %s\n",
				 "gen_ddr_timing", strerror(errno));
			goto err;
		}
		if (timing[++i].name == NULL)
			break;
	}

	if (write_file(fd, CONFIG_TAIL, strlen(CONFIG_TAIL)) < 0) {
		fprintf (stderr, "%s: Can't write TAIL: %s\n",
			 "gen_ddr_timing", strerror(errno));
	}
	fsync(fd);
err:
	free(buf);
	close(fd);

	return 0;
}
