folder:=`basename $(CURDIR)`

sp1: part1.cc
	g++ -std=c++14 part1.cc -o part1
	./part1 sample.in

ap1: part1.cc
	g++ -std=c++14 part1.cc -o part1
	./part1 $(folder).in

sp2: part2.cc
	g++ -std=c++14 part2.cc -o part2
	./part2 sample.in

ap2: part2.cc
	g++ -std=c++14 part2.cc -o part2
	./part2 $(folder).in

copy: part1.cc part2.cc
	cp part1.cc part2.cc