test: main.o
	g++ -o test main.o -std=c++17 -O2 -W

main.o: src/main.cpp include/*.h
	g++ -o main.o -c src/main.cpp -std=c++17 -O2 -W

clean:
	rm -rf test main.o
