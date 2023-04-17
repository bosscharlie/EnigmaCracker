CPP_FLAGS=-std=c++11

main: EnigmaCracker.o Enigma.o
	g++ ${CPP_FLAGS} EnigmaCracker.o Enigma.o -lpthread -o main
EnigmaCracker.o: EnigmaCracker.cpp EnigmaCracker.h
	g++ ${CPP_FLAGS} -c EnigmaCracker.cpp 
Enigma.o: Enigma.cpp Enigma.h
	g++ ${CPP_FLAGS} -c Enigma.cpp 
clean:
	rm -f *.o main *.txt