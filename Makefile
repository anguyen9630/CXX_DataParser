
dep_outputs := scaledataparser.o

scaleparser: $(dep_outputs)
	g++ src/main.cpp -Iinclude -o scaleparser $(dep_outputs)

scaledataparser.o:
	g++ -c src/scaledataparser.cpp -Iinclude -o scaledataparser.o

clean:
	rm -rf $(dep_outputs) scaleparser

