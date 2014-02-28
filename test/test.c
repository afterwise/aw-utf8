
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include "aw-utf8.h"
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
		printf("  at offset %ld\n", sub - str);
		printf(
			"  bytes: %02x %02x %02x %02x %02x ...\n\n",
			sub[0], sub[1], sub[2], sub[3], sub[4]);
		return -1;
	}

	if (sub - str != len) {
		printf(" Scanned short!\n");
		printf("  scan length: %ld\n", sub - str);
		printf("  string length: %ld\n", len);
		return -1;
	}

	printf(" OK!\n");
	return 0;
}

int main(int argc, char *argv[]) {
	const char **it;
	struct stat stat;
	void *ptr;
	char *str;
	int fd, ne = 0, nf = 0;

	for (it = files; *it != NULL; ++it) {
		printf("Reading file\n");
		printf(" path: %s\n", *it);

		if (stat.st_size > 1024ull * 1024ull)
			printf(" size: %llu MB\n", stat.st_size / 1024ull / 1024ull);
		else if (stat.st_size > 1024ull)
			printf(" size: %llu KB\n", stat.st_size / 1024ull);
		else
			printf(" size: %llu B\n", stat.st_size);

		fd = open(*it, O_RDONLY);
		fstat(fd, &stat);
		ptr = mmap(NULL, stat.st_size, PROT_READ, MAP_FILE | MAP_PRIVATE, fd, 0);
		close(fd);

		str = malloc(stat.st_size + 1);
		memcpy(str, ptr, stat.st_size);
		str[stat.st_size] = 0;
		munmap(ptr, stat.st_size);

		if (scan(str, stat.st_size) < 0)
			++ne;

		++nf;
		free(str);
	}

	printf("%d errors\n", ne);
	printf("%d files\n", nf);

	return 0;
}

