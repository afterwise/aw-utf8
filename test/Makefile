
export CFLAGS += -Wall -Wextra

test: test.o ../libaw-utf8.a
	clang -o $@ $^

%.o: %.c files.h
	clang $(CFLAGS) -I.. -c $< -o $@

../libaw-utf8.a:
	$(MAKE) -C..

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
	rm -rf UTF-8-Unicode-Test-Documents

