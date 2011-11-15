CXXFLAGS=       -Wall $(COPT)

all:
	$(CXX) -o main main.cpp -I . $(CFLAGS) -O2

debug:
	$(CXX) -o main main.cpp -I . $(CFLAGS) -O2 -g

test:
	$(CXX) -o tests/driver tests/driver.cpp -I . $(CXXFLAGS)
	tests/driver

clean:
	rm main tests/driver
