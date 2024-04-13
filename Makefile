build/test_runner: tests/*.cpp include/*.h build/catch2.o
	g++ -g -std=c++17 -o build/test_runner -Iinclude tests/*.cpp -Ilib build/catch2.o

build/catch2.o: lib/catch_amalgamated.cpp lib/catch_amalgamated.hpp
	cd build && g++ -g -std=c++17 -c ../lib/catch_amalgamated.cpp -I../lib/** -o catch2.o

run_tests: build/test_runner
	./build/test_runner


