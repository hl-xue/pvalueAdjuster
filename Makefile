target: pvalueAdjuster

pvalueAdjuster: pvalueAdjuster.cpp
	g++ -o pvalueAdjuster pvalueAdjuster.cpp -lboost_iostreams -Wall -Werror -g -std=c++14
