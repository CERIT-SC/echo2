CXXFLAGS = -std=c++11 -pthread -O3 -m64

.PHONY: all clean install uninstall

all: echo2

clean:
	-rm echo2

install:
	mv echo2 /usr/local/bin/echo2
	@echo "Echo has been successfully installed (copied to /usr/local/bin/)"

uninstall:
	rm /usr/local/bin/echo2
	@echo "Echo successfully uninstalled"

echo2: *.cpp
	$(CXX) $(CXXFLAGS) -o $@ $?
	@echo "Echo has been successfully compiled"