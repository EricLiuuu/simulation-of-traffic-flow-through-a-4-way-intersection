all: proj5.exe

random.o: random.h random.cpp
	g++ -std=c++98 -g -Wall -c random.cpp -o random.o

EventClass.o: constants.h EventClass.h EventClass.cpp
	g++ -std=c++98 -g -Wall -c EventClass.cpp -o EventClass.o

CarClass.o: constants.h CarClass.h CarClass.cpp
	g++ -std=c++98 -g -Wall -c CarClass.cpp -o CarClass.o

IntersectionSimulationClass.o: IntersectionSimulationClass.h SortedListClass.h EventClass.h FIFOQueueClass.h CarClass.h LinkedNodeClass.h IntersectionSimulationClass.cpp
	g++ -std=c++98 -g -Wall -c IntersectionSimulationClass.cpp -o IntersectionSimulationClass.o 

project5.o: project5.cpp IntersectionSimulationClass.h
	g++ -std=c++98 -g -Wall -c project5.cpp -o project5.o

proj5.exe: project5.o random.o EventClass.o CarClass.o IntersectionSimulationClass.o
	g++ -std=c++98 project5.o random.o EventClass.o CarClass.o IntersectionSimulationClass.o -o proj5.exe

clean:
	rm -f project5.o random.o EventClass.o CarClass.o IntersectionSimulationClass.o proj5.exe


