all: mylife

mylife: src/mylife.cpp src/Grammar.h src/Grammar.cpp
		mpic++ -I. src/mylife.cpp src/Grammar.cpp  -o mylife -std=c++11
		#g++ -Werror -Wall -O3  -I. src/featurized_fast_align.cpp src/FMaps.cpp -o featureized_fast_align  -std=c++11 #-g -O1 #-DDEBUG
clean:
	    rm -f mylife
