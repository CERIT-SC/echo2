.PHONY: all configure clean install uninstall here

export PREFIX ?= /usr/local

all:
	$(MAKE) -C Echo all

depend:
	$(MAKE) -C Echo depend

clean:
	$(MAKE) -C Echo clean
	rm -f echo2
	@echo "Directory is clean"

install:
	$(MAKE) -C Echo install

uninstall:
	$(MAKE) -C Echo uninstall

here:
	cp Echo/echo2 echo2
