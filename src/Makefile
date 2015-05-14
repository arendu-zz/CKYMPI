all: ckympi

ckympi: ckympi.cpp Grammar.h Grammar.cpp Message.h Message.cpp
		mpic++ -I. ckympi.cpp Grammar.cpp Message.cpp -o ckympi -std=c++11
clean:
	    rm -f ckympi
