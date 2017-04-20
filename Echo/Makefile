CXXFLAGS = -std=c++11 -pthread -O3 -m64
OBJ := $(patsubst %.cpp,%.o,$(wildcard *.cpp))


all: echo2

depend:
	$(CXX) $(CXXFLAGS) -M *.cpp > Makefile-depend
-include Makefile-depend

clean:
	rm -f *.o echo2

install:
	mkdir -p $(PREFIX)/bin
	cp echo2 $(PREFIX)/bin/echo2
	@echo "Echo has been successfully installed (copied to $(PREFIX)/bin/)"

uninstall:
	rm $(PREFIX)/bin/echo2
	@echo "Echo successfully uninstalled"

.PHONY: all configure clean install uninstall




echo2: $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $(OBJ)
	@echo "Echo has been successfully compiled"