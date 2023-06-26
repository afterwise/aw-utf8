
#include "aw-utf8.h"
#include "aw-fs.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "files.h"

static int scan(const char *str, size_t len) {
	const char *sub = str;
	unsigned c, nc = 0;
	int n;

	while ((n = utf8_read(&c, sub)) > 0) {
		if (c == 0 && sub - str != 0)
			break;

		sub += n;
		++nc;
	}

	if (n < 0) {
		printf(" Scanning failed!\n");
		printf("  at offset %ld\n", (long) (sub - str));
		printf(
			"  bytes: %02x %02x %02x %02x %02x ...\n\n",
			sub[0], sub[1], sub[2], sub[3], sub[4]);
		return -1;
	}

	if ((size_t) (sub - str) != len) {
		printf(" Scanned short!\n");
		printf("  scan length: %ld\n", (long) (sub - str));
		printf("  string length: %ld\n", (long) len);
		return -1;
	}

	printf(" OK!\n");
	return 0;
}

int main(int argc, char *argv[]) {
	const char **it;
	fs_stat_t st;
	struct fs_map map;
	void *ptr;
	char *str;
	int ne = 0, nf = 0;

	(void) argc;
	(void) argv;

	for (it = files; *it != NULL; ++it) {
		printf("Reading file\n");
		printf(" path: %s\n", *it);

		if (st.st_size > 1024ll * 1024ll)
			printf(" size: %ld MB\n", (unsigned long) (st.st_size / 1024ll / 1024ll));
		else if (st.st_size > 1024ll)
			printf(" size: %ld KB\n", (unsigned long) (st.st_size / 1024ll));
		else
			printf(" size: %ld B\n", (unsigned long) st.st_size);

		ptr = fs_map(&map, *it);

		str = malloc(map.size + 1);
		memcpy(str, ptr, map.size);
		str[map.size] = 0;

		fs_unmap(&map);

		if (scan(str, map.size) < 0)
			++ne;

		++nf;
		free(str);
	}

	printf("%d errors\n", ne);
	printf("%d files\n", nf);

	return 0;
}

