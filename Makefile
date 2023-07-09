dep_outputs := scaledataparser.o utils.o

scaleparser: $(dep_outputs)
	g++ src/main.cpp -Iinclude -o scaleparser $(dep_outputs)

scaledataparser.o: utils.o
	g++ -c src/scaledataparser.cpp -Iinclude -o scaledataparser.o

utils.o:
	g++ -c src/utils.cpp -Iinclude -o utils.o

clean:
	rm -rf $(dep_outputs) scaleparser

