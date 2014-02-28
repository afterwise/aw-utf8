
#ifndef AW_UTF8_H
#define AW_UTF8_H

#include <stddef.h>

#if __GNUC__
# define _utf8_alwaysinline inline __attribute__((always_inline, nodebug))
#elif _MSC_VER
# define _utf8_alwaysinline __forceinline
#endif

#ifdef __cplusplus
extern "C" {
#endif

int utf8_size(unsigned chr);
int utf8_read(unsigned *chr, const char *str);
int utf8_write(char *str, unsigned chr);

struct utf8_iter {
	char *tail;
	char *head;
	unsigned chr;
};

_utf8_alwaysinline void utf8_iter(struct utf8_iter *it, char *str) {
	it->tail = str;
	it->head = NULL;
	it->chr = 0;
}

unsigned utf8_next(struct utf8_iter *it);

size_t utf8_copy(char *dst, char *src, size_t size);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* AW_UTF8_H */

