
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

		if (c != 0) {
			wchar_t wc[2];
			memset(wc, 0, sizeof wc);
			int err = utf8_to_ucs2(sub, n, wc, 2);
			if (err != 1 || ((unsigned) (unsigned short) wc[0] & 0xffff) != (c & 0xffff)) {
				printf(" Scanning failed! (utf8_to_ucs2)\n");
				printf("  at offset %ld\n", (long) (sub - str));
				printf("  err: %d (%d)\n", err, 1);
				printf("  ucs2: %x\n", (unsigned) (unsigned short) wc[0]);
				printf("  ucs4: %x\n", c);
				printf(
					"  bytes: %02x %02x %02x %02x %02x ...\n\n",
					sub[0] & 0xff, sub[1] & 0xff, sub[2] & 0xff, sub[3] & 0xff, sub[4] & 0xff);
				return -1;
			}

			// if ucs4 is not out of range for ucs2
			if ((unsigned) (unsigned short) wc[0] == c) {
				char bc[8];
				memset(bc, 0, sizeof bc);
				err = utf8_from_ucs2(wc, 1, bc, 8);
				if (err != n || memcmp(sub, bc, n) != 0) {
					printf(" Scanning failed! (utf8_from_ucs2)\n");
					printf("  at offset %ld\n", (long) (sub - str));
					printf("  err: %d (%d)\n", err, n);
					printf("  ucs2: %x\n", (unsigned) (unsigned short) wc[0]);
					printf("  ucs4: %x\n", c);
					printf(
						"  before: %02x %02x %02x %02x %02x ...\n",
						sub[0] & 0xff, sub[1] & 0xff, sub[2] & 0xff, sub[3] & 0xff, sub[4] & 0xff);
					printf(
						"  after: %02x %02x %02x %02x %02x ...\n\n",
						bc[0] & 0xff, bc[1] & 0xff, bc[2] & 0xff, bc[3] & 0xff, bc[4] & 0xff);
					return -1;
				}
			}
		}

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
	struct fs_map map;
	void *ptr;
	char *str;
	int ne = 0, nf = 0;

	(void) argc;
	(void) argv;

	for (it = files; *it != NULL; ++it) {
		printf("Reading file\n");
		printf(" path: %s\n", *it);

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

