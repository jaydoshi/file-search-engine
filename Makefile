all: crawler search2 search

crawler: crawler.cpp
	g++ -std=c++11 -Wall -g crawler.cpp -o $@

search2: search2.cpp
	g++ -std=c++11 -Wall -g search2.cpp -o $@

search: search2.cpp
	g++ -std=c++11 -Wall -g search2.cpp -o $@
