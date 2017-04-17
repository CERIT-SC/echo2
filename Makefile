.PHONY: all clean install uninstall

all:
	$(MAKE) -C Echo all

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
