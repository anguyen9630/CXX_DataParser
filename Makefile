dep_outputs := scaledataparser.o utils.o serialdriver.o

scaleparser: $(dep_outputs)
	g++ src/main.cpp -ljsoncpp -std=c++11 -Iinclude -o scaleparser $(dep_outputs)
	rm *.o

scaledataparser.o: utils.o
	g++ -c src/scaledataparser.cpp -std=c++11 -Iinclude -o scaledataparser.o

serialdriver.o: utils.o
	g++ -c src/serialdriver.cpp -std=c++11 -Iinclude -o serialdriver.o

utils.o:
	g++ -c src/utils.cpp -std=c++11 -Iinclude -o utils.o

clean:
	rm -rf $(dep_outputs) scaleparser

