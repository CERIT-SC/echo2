export PREFIX ?= /usr/local


.PHONY: all
all:
	$(MAKE) -C Echo all

.PHONY: depend
depend:
	$(MAKE) -C Echo depend

.PHONY: clean
clean:
	$(MAKE) -C Echo clean
	rm -f echo2
	@echo "Directory is clean"

.PHONY: install
install:
	$(MAKE) -C Echo install

.PHONY: uninstall
uninstall:
	$(MAKE) -C Echo uninstall

.PHONY: here
here:
	cp Echo/echo2 echo2
