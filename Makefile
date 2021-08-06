#
# Copyright 2021 Josua Mayer <josua.mayer@jm0.eu>
#

CC ?= cc
DESTDIR ?= 
PREFIX ?= /usr/local

all: btt

btt: btt.c
	$(CC) -o $@ -lgpiod $<

install: btt btt-reset.sh imx6qdl-hummingboard.jinja2
	install -v -m755 -o root -g root btt $(DESTDIR)$(PREFIX)/sbin/
	install -v -m755 -o root -g root btt-reset.sh $(DESTDIR)$(PREFIX)/sbin/btt-reset
	install -v -m644 -o root -g root imx6qdl-hummingboard.jinja2 $(DESTDIR)/etc/lava-server/dispatcher-config/device-types/

clean:
	rm -fv btt
