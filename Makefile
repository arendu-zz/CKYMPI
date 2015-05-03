
all: mylife

mylife: src/mylife.cpp src/Grammar.h src/Grammar.cpp src/Message.h src/Message.cpp
		mpic++ -I. src/mylife.cpp src/Grammar.cpp src/Message.cpp -o mylife -std=c++11
		#g++ -Werror -Wall -O3  -I. src/featurized_fast_align.cpp src/FMaps.cpp -o featureized_fast_align  -std=c++11 #-g -O1 #-DDEBUG
clean:
	    rm -f mylife
