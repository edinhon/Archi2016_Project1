simulator: simulator.o instruction.o regfile.o memory.o
	g++ -o simulator simulator.o instruction.o regfile.o memory.o
simulator.o: simulator.cpp instruction.h regfile.h memory.h
	g++ -c simulator.cpp
instruction.o: instruction.cpp instruction.h regfile.h
	g++ -c instruction.cpp
regfile.o: regfile.cpp regfile.h
	g++ -c regfile.cpp
memory.o: memory.cpp memory.h
	g++ -c memory.cpp
clean:
	rm -f simulator simulator.o instruction.o regfile.o memory.o
