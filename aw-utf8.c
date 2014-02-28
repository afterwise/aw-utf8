
#include "aw-utf8.h"

#if __GNUC__
# define _utf8_likely(x) __builtin_expect(!!(x), 1)
# define _utf8_unlikely(x) __builtin_expect(!!(x), 0)
#else
# define _utf8_likely(x) (x)
# define _utf8_unlikely(x) (x)
#endif

int utf8_size(unsigned chr) {
	return chr < 0x80 ? 1 : chr < 0x800 ? 2 : chr < 0x10000 ? 3 : 4;
}

int utf8_read(unsigned *chr, const char *str) {
	if ((unsigned char) str[0] < 0x80) {
		*chr = (unsigned char) str[0];
		return 1;
	}

	if (((unsigned char) str[0] >= 0xb0) && ((unsigned char) str[0] < 0xe0)) {
		*chr = (((unsigned char) str[0] & 0x1f) << 6) |
			((unsigned char) str[1] & 0x3f);
		return 2;
	}

	if (((unsigned char) str[0] >= 0xe0) && ((unsigned char) str[0] < 0xf0)) {
		*chr = (((unsigned char) str[0] & 0xf) << 12) |
			(((unsigned char) str[1] & 0x3f) << 6) |
			((unsigned char) str[2] & 0x3f);
		return 3;
	}

	if (!((unsigned char) str[0] & 0x8)) {
		*chr = (((unsigned char) str[0] & 0x7) << 18) |
			(((unsigned char) str[1] & 0x3f) << 12) |
			(((unsigned char) str[2] & 0x3f) << 6) |
			((unsigned char) str[3] & 0x3f);
		return 4;
	}

	return -1;
}

int utf8_write(char *str, unsigned chr) {
	if (chr < 0x80) {
		str[0] = (char) chr;
		return 1;
	}

	if (chr < 0x0800) {
		str[0] = 0xc0 | (char) (chr >> 6);
		str[1] = 0x80 | (char) (chr & 0x3f);
		return 2;
	}

	if (chr < 0x10000) {
		str[0] = 0xe0 | (char) (chr >> 12);
		str[1] = 0x80 | (char) ((chr >> 6) & 0x3f);
		str[2] = 0x80 | (char) (chr & 0x3f);
		return 3;
	}

	str[0] = 0xf0 | (char) (chr >> 18);
	str[1] = 0x80 | (char) ((chr >> 12) & 0x3f);
	str[2] = 0x80 | (char) ((chr >> 6) & 0x3f);
	str[3] = 0x80 | (char) (chr & 0x3f);
	return 4;
}

unsigned utf8_next(struct utf8_iter *it) {
	unsigned c = 0;
	int n = utf8_read(&c, it->tail);

	it->head = it->tail;
	it->chr = c;

	if (n > 0)
		it->tail += n;

	return c;
}

size_t utf8_copy(char *dst, char *src, size_t size) {
	size_t len = 0;
	unsigned c;
	int n;

	while ((n = utf8_read(&c, src)) > 0) {
		if (_utf8_unlikely(len + n > size))
			return size;

		utf8_write(dst, c);

		if (_utf8_unlikely(c == 0))
			break;

		src += n;
		dst += n;
		len += n;
	}

	return len;
}
