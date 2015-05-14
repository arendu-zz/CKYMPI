all: ckympi

ckympi: src/ckympi.cpp src/Grammar.h src/Grammar.cpp src/Message.h src/Message.cpp
		mpic++ -I. src/ckympi.cpp src/Grammar.cpp src/Message.cpp -o ckympi -std=c++11
clean:
	    rm -f ckympi
