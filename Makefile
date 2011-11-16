CXXFLAGS=       -Wall $(COPT)

all:
	$(CXX) -o main src/main.cpp -I . $(CFLAGS) -O2

debug:
	$(CXX) -o main src/main.cpp -I . $(CFLAGS) -O2 -g -DDEBUG

test:
	$(CXX) -o tests/driver tests/driver.cpp -I . $(CXXFLAGS) -DDEBUG
	tests/driver

clean:
	rm main tests/driver
