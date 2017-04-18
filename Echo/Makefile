CXXFLAGS = -std=c++11 -O3 -m64
OBJ := $(patsubst %.cpp,%.o,$(wildcard *.cpp))


all: echo2

depend:
	$(CXX) $(CXXFLAGS) -M *.cpp > Makefile-dependencies
-include Makefile-dependencies

clean:
	-rm *.o echo2

install:
	mv echo2 /usr/local/bin/echo2
	@echo "Echo has been successfully installed (copied to /usr/local/bin/)"

uninstall:
	rm /usr/local/bin/echo2
	@echo "Echo successfully uninstalled"

.PHONY: all configure clean install uninstall




echo2: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $?
	@echo "Echo has been successfully compiled"