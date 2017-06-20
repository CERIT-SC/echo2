CXXFLAGS = -std=c++11 -pthread -O3 -m64
LDFLAGS += -pthread -ltcmalloc
OBJ := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

.PHONY: all
all: echo2

.PHONY: depend
depend:
	$(CXX) $(CXXFLAGS) -M *.cpp > Makefile-depend
-include Makefile-depend

.PHONY: clean
clean:
	rm -f *.o echo2

.PHONY: install
install:
	mkdir -p $(prefix)/bin
	cp echo2 $(prefix)/bin/echo2
	@echo "Echo has been successfully installed (copied to $(prefix)/bin/)"

.PHONY: uninstall
uninstall:
	rm $(prefix)/bin/echo2
	@echo "Echo successfully uninstalled"


echo2: $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $^
	@echo "Echo has been successfully compiled"