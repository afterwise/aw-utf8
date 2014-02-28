
/*
   Copyright (c) 2014 Malte Hildingsson, malte (at) afterwi.se

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
 */

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

