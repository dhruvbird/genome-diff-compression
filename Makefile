CXXFLAGS=       -Wall $(COPT)

all:
	$(CXX) -o main src/main.cpp -I . $(CFLAGS) -O2
	$(CXX) -o sanitize src/sanitize.cpp -I . $(CFLAGS) -O2

t2b:
	$(CXX) -o text2binary src/text2binary.cpp -I . $(CFLAGS) -O2

debug:
	$(CXX) -o main src/main.cpp -I . $(CFLAGS) -O2 -g -DDEBUG

test:
	$(CXX) -o tests/driver tests/driver.cpp -I . $(CXXFLAGS) -DDEBUG
	tests/driver

clean:
	rm -f main tests/driver text2binary sanitize
