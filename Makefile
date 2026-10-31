CC ?= cc
PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin
DATADIR ?= $(PREFIX)/share
MANDIR ?= $(DATADIR)/man
UDEVDIR ?= /etc/udev/rules.d
CFLAGS ?= -O2 -pipe
CPPFLAGS += -Iinclude -D_POSIX_C_SOURCE=200809L
CFLAGS += -std=c11 -Wall -Wextra -Wpedantic
LDFLAGS ?=

PROGRAM = nk68_ctl
COMMON_SOURCES = src/main.c src/cli.c src/nk68.c
UNAME_S := $(shell uname -s 2>/dev/null)
ifeq ($(UNAME_S),Linux)
PLATFORM_SOURCE = src/device_linux.c
else
PLATFORM_SOURCE = src/device_stub.c
endif
OBJECTS = $(COMMON_SOURCES:.c=.o) $(PLATFORM_SOURCE:.c=.o)

.PHONY: all clean install uninstall test

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

src/%.o: src/%.c include/cli.h include/nk68.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(OBJECTS:.o=.d)

test: $(PROGRAM)
	./$(PROGRAM) --help >/dev/null
	./$(PROGRAM) 13 --help >/dev/null

install: $(PROGRAM)
	install -Dm755 $(PROGRAM) "$(DESTDIR)$(BINDIR)/$(PROGRAM)"
	install -Dm644 completions/nk68_ctl.bash \
		"$(DESTDIR)$(DATADIR)/bash-completion/completions/nk68_ctl"
	install -Dm644 man/nk68_ctl.1 "$(DESTDIR)$(MANDIR)/man1/nk68_ctl.1"
	install -Dm644 udev/99-nk68.rules \
		"$(DESTDIR)$(UDEVDIR)/99-nk68.rules"

uninstall:
	rm -f "$(DESTDIR)$(BINDIR)/$(PROGRAM)"
	rm -f "$(DESTDIR)$(DATADIR)/bash-completion/completions/nk68_ctl"
	rm -f "$(DESTDIR)$(MANDIR)/man1/nk68_ctl.1"
	rm -f "$(DESTDIR)$(UDEVDIR)/99-nk68.rules"

clean:
	rm -f $(PROGRAM) $(OBJECTS) $(OBJECTS:.o=.d)
