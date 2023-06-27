STD=--std=c++17 -pthread
GCC=g++
OBJ=obj
BIN=bin

bin/race: src/main.cpp obj/race.o obj/semaphore.o
	[ -d $(BIN) ] || mkdir -p $(BIN)
	${GCC} ${STD} -o ${BIN}/race src/main.cpp obj/race.o obj/semaphore.o

obj/race.o: src/race.cpp src/race.h
	[ -d ${OBJ} ] || mkdir -p ${OBJ}
	${GCC} ${STD} -c -o ${OBJ}/race.o src/race.cpp

obj/semaphore.o: src/semaphore.cpp src/semaphore.h
	[ -d ${OBJ} ] || mkdir -p ${OBJ}
	${GCC} ${STD} -c -o ${OBJ}/semaphore.o src/semaphore.cpp

.PHONY: doc run clean

run: bin/race
	./bin/race 2
	./bin/race 4
	./bin/race 10

doc:
	doxygen config
	cd latex && make

clean:
	rm -f obj/*.o
	rm -f bin/race
	rm -r -f bin
	rm -r -f obj
	rm -r -f html
	rm -r -f latex
