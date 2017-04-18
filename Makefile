.PHONY: all configure clean install uninstall here

all:
	$(MAKE) -C Echo all

configure:
	$(MAKE) -C Echo configure

clean:
	$(MAKE) -C Echo clean
	-rm echo2
	@echo "Directory is clean"

install:
	$(MAKE) -C Echo install

uninstall:
	$(MAKE) -C Echo uninstall

here:
	mv Echo/echo2 echo2
