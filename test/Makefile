
export CFLAGS += -Wall -Wextra

test: test.o ../libaw-utf8.a aw-fs/libaw-fs.a
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c files.h aw-fs/aw-fs.h
	$(CC) $(CFLAGS) -I.. -Iaw-fs -c $< -o $@

../libaw-utf8.a:
	$(MAKE) -C..

aw-fs/libaw-fs.a:
	$(MAKE) -Caw-fs

aw-fs/aw-fs.h:
	git clone git@github.com:afterwise/aw-fs.git && \
	touch -c $@

files.h: UTF-8-Unicode-Test-Documents/README.md
	echo "const char *files[] = {" > files.h
	ls UTF-8-Unicode-Test-Documents/UTF-8_sequence_unseparated/*.txt | \
	xargs printf "\t\"%s\",\n" >> files.h
	echo "\tNULL\n};" >> files.h

UTF-8-Unicode-Test-Documents/README.md:
	git clone git@github.com:bits/UTF-8-Unicode-Test-Documents.git && \
	touch -c $@

.PHONY: clean
clean:
	rm -f test test.o files.h
	rm -rf aw-fs UTF-8-Unicode-Test-Documents

